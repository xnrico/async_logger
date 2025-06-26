#include "async_worker.hpp"

namespace ricox {
async_worker::async_worker(const std::function<void(async_buffer&)> consumer_callback_)
	: consumer_callback(consumer_callback_),
	  worker([this]() -> void { this->async_worker::thread_func(); }),
	  is_stopped(false) {}

async_worker::async_worker(const std::function<void(async_buffer&)> consumer_callback_, size_t buffer_size_)
	: producer_buffer(buffer_size_),
	  consumer_buffer(buffer_size_),
	  consumer_callback(consumer_callback_),
	  worker([this]() -> void { this->async_worker::thread_func(); }),
	  is_stopped(false) {}

// background thread that swaps buffers and notify the actors involved
auto async_worker::thread_func() -> void {
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
		consumer_callback(consumer_buffer);	 // consumer reads the buffer
		consumer_buffer.reset();			 // reset buffer after read is complete
	}
}

async_worker::~async_worker() {
	is_stopped.store(true);
	consumer_cv.notify_all();
	worker.join();
}

auto async_worker::write(std::string_view sv) -> void {
	auto lock = std::unique_lock<std::mutex>(common_mtx);

	// block until the buffer is writable
	producer_cv.wait(lock, [&]() -> bool { return sv.size() <= producer_buffer.get_writable_size(); });
	producer_buffer.write(sv);
	consumer_cv.notify_one();
}
}  // namespace ricox