#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

namespace std {
using cv = condition_variable;
}

namespace ricox {
class thread_pool {
   private:
	std::vector<std::thread> threads;
	std::queue<std::function<void()>> tasks;
	std::mutex q_mtx;
	std::cv q_cv;

	size_t capacity;
	bool is_stopped;

   public:
	thread_pool() = delete;
	thread_pool(const thread_pool&) = delete;
	thread_pool& operator=(const thread_pool&) = delete;
	thread_pool(thread_pool&&) = delete;
	thread_pool&& operator=(thread_pool&&) = delete;

	thread_pool(size_t capacity_);
	~thread_pool();

	// Templated functions can only be defined here
	template <typename F, typename... Args>
	auto add_task(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type> {
		using return_type = typename std::result_of<F(Args...)>::type;
		auto result = std::future<return_type>{};

		// first package task into returning futures before entering the critical section
		// it is created as a shared pointer because its lifetime is beyond the add_task function
		auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

		{  // critical section
			// when thread pool is running, add thread to the queue
			auto lock = std::unique_lock<std::mutex>(q_mtx);
			if (is_stopped) {
				throw std::runtime_error("ERROR: Adding task to a STOPPED thread pool");
			}

			tasks.emplace([task]() -> void { (*task)(); });	 // add task to queue as a void()
		}

		q_cv.notify_one();	// notify a thread to start this task
		return result;
	}
};
}  // namespace ricox