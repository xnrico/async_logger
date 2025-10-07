#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>

#include "async_buffer.hpp"
#include "lf_buffer.hpp"

namespace std {
using cv = condition_variable;
}

namespace ricox {

class async_worker {
 protected:
  std::thread worker;
  std::atomic<bool> is_stopped;

 public:
  async_worker();
  virtual ~async_worker() = 0;

  async_worker(const async_worker&) = delete;
  async_worker& operator=(const async_worker&) = delete;
  async_worker(async_worker&&) = delete;
  async_worker&& operator=(async_worker&&) = delete;

  virtual auto write(std::string_view sv) -> void = 0;
};

// multi-consumer, multi-producer compatible (mutex and cv)
class async_worker_mcv final : public async_worker {
 private:
  async_buffer producer_buffer;
  async_buffer consumer_buffer;
  std::mutex common_mtx;
  std::cv producer_cv;
  std::cv consumer_cv;

  std::function<void(async_buffer&)> consumer_callback;

  auto thread_func() -> void;

 public:
  async_worker_mcv() = delete;
  async_worker_mcv(const std::function<void(async_buffer&)> consumer_callback_);
  async_worker_mcv(const std::function<void(async_buffer&)> consumer_callback_, size_t buffer_size_);
  ~async_worker_mcv() override;

  async_worker_mcv(const async_worker&) = delete;
  async_worker_mcv& operator=(const async_worker&) = delete;
  async_worker_mcv(async_worker_mcv&&) = delete;
  async_worker_mcv&& operator=(async_worker_mcv&&) = delete;

  auto write(std::string_view sv) -> void override;
};

// single-consumer, single-producer (lock-free)
class async_worker_lf final : public async_worker {
 private:
  lf_buffer<std::string, 4096> buffer_;  // lock free ring buffer of 4KB

  std::function<void(lf_buffer<std::string, 4096>&)> consumer_callback;

  auto thread_func() -> void;

 public:
  async_worker_lf() = delete;
  async_worker_lf(const std::function<void(lf_buffer<std::string, 4096>&)> consumer_callback_);
  ~async_worker_lf() override;

  async_worker_lf(const async_worker&) = delete;
  async_worker_lf& operator=(const async_worker&) = delete;
  async_worker_lf(async_worker_lf&&) = delete;
  async_worker_lf&& operator=(async_worker_lf&&) = delete;

  auto write(std::string_view sv) -> void override;
};

}  // namespace ricox