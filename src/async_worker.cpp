#include "async_worker.hpp"

namespace ricox {

// base class
async_worker::async_worker() : is_stopped(true) {}

async_worker::~async_worker() {}  // do not use default destructor, otherwise deadlock may occur

// MPMC class
async_worker_mcv::async_worker_mcv(const std::function<void(async_buffer&)> consumer_callback_, size_t buffer_size_)
    : consumer_callback(consumer_callback_), producer_buffer(buffer_size_), consumer_buffer(buffer_size_) {
  is_stopped.store(false);
  worker = std::thread([this]() -> void { this->thread_func(); });
}

async_worker_mcv::async_worker_mcv(const std::function<void(async_buffer&)> consumer_callback_)
    : async_worker_mcv(consumer_callback_, 4096) {}

async_worker_mcv::~async_worker_mcv() {
  is_stopped.store(true);
  consumer_cv.notify_all();
  worker.join();
}

// background thread that swaps buffers and notify the actors involved
auto async_worker_mcv::thread_func() -> void {
  while (true) {
    {
      // consumer buffer is empty at the beginning of the loop, consumer waits for producer to produce data
      auto lock = std::unique_lock<std::mutex>(common_mtx);  // prevent consumers from reading
      // wakes up when data is produced or when the worker is stopped
      consumer_cv.wait(lock, [&]() -> bool { return !producer_buffer.empty() || is_stopped.load(); });

      if (is_stopped.load() && producer_buffer.empty()) {
        // is woken up because of stopped worker, wake up all waiting producers and exit
        producer_cv.notify_all();
        return;
      }

      // exchange as soon as there is data available for read
      producer_buffer.swap(consumer_buffer);
      producer_cv.notify_one();  // now producer buffer should have available space, notify blocking producers
    }

    // now consumer buffer gets data (but might be empty)
    consumer_callback(consumer_buffer);  // consumer reads the buffer
    consumer_buffer.reset();             // reset buffer after read is complete
  }
}

auto async_worker_mcv::write(std::string_view sv) -> void {
  auto lock = std::unique_lock<std::mutex>(common_mtx);

  // block until the buffer is writable
  producer_cv.wait(lock, [&]() -> bool { return sv.size() <= producer_buffer.get_writable_size(); });
  producer_buffer.write(sv);
  consumer_cv.notify_one();
}

// SPSC class
async_worker_lf::async_worker_lf(const std::function<void(lf_buffer<std::string, 4096>&)> consumer_callback_)
    : consumer_callback(consumer_callback_) {
  is_stopped.store(false);
  worker = std::thread([this]() -> void { this->thread_func(); });
}

async_worker_lf::~async_worker_lf() {
  is_stopped.store(true);
  worker.join();
}

// background thread that swaps buffers and notify the actors involved
auto async_worker_lf::thread_func() -> void {
  while (!is_stopped.load()) {
    while (!buffer_.empty()) {
      consumer_callback(buffer_);
    }

    std::this_thread::sleep_for(std::chrono::microseconds(10));  // avoid busy waiting
  }
}

auto async_worker_lf::write(std::string_view sv) -> void {
  while (!buffer_.try_push(std::string{sv})) {
    std::this_thread::sleep_for(std::chrono::microseconds(10));  // wait and retry
  }
}

}  // namespace ricox