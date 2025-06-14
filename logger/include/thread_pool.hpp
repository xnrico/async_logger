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
		
		// The inner lambda `[f_captured, args_captured]() mutable` is what the packaged_task will execute
        // Its signature is void() because it takes no arguments itself.
        // It captures 'f_captured' and 'args_captured' by value (which are already perfectly forwarded once)
        // and then uses std::apply to call the original function 'f' with 'args'.

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            [f_captured = std::forward<F>(f), args_captured = std::make_tuple(std::forward<Args>(args)...)]() {
                // When this lambda is called (by the thread pool), it will
                // execute the original function 'f_captured' with its 'args_captured'.
                // std::apply handles unpacking the tuple and passing arguments with correct value categories.
                return std::apply(f_captured, args_captured);
            }
        );

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