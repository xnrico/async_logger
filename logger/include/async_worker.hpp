#pragma once

#include "async_buffer.hpp"

#include <condition_variable>
#include <atomic>
#include <functional>
#include <mutex>
#include <thread>

namespace std {
using cv = condition_variable;
}

namespace ricox {
class async_worker {
   private:
	async_buffer producer_buffer;
	async_buffer consumer_buffer;
	std::mutex common_mtx;
	std::cv producer_cv;
	std::cv consumer_cv;

	std::thread worker;
	std::function<void(async_buffer&)> consumer_callback;

    std::atomic<bool> is_stopped;

    auto thread_func() -> void;

   public:
	async_worker() = delete;
    async_worker(const std::function<void(async_buffer&)> consumer_callback_);
	async_worker(const std::function<void(async_buffer&)> consumer_callback_, size_t buffer_size_);
	~async_worker();

	async_worker(const async_worker&) = delete;
	async_worker& operator=(const async_worker&) = delete;
	async_worker(async_worker&&) = delete;
	async_worker&& operator=(async_worker&&) = delete;

    auto write(std::string_view sv) -> void;
};
}  // namespace ricox