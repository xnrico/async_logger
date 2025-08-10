#include "thread_pool.hpp"

namespace ricox {
thread_pool::thread_pool(size_t capacity_) : capacity(capacity_), is_stopped(false) {
	// reserve memory space for the thread objects
	this->threads.reserve(capacity);

	// create new threads in thread pool up to its capacity (for example 3 threads)
	// for each thread:
	// enter an infinite loop to get tasks from the queue if they become available
	// if the thread pool is stopped and queue is empty, exit the loop

	for (auto i = 0; i < capacity; ++i) {
		this->threads.emplace_back([this]() -> void {
			// job for each thread
			for (;;) {
				auto task = std::function<void()>{};

				{													  // critical Section
					auto lock = std::unique_lock<std::mutex>{q_mtx};  // using unique_lock so it does not lock immediately
					q_cv.wait(lock, [this]() -> bool { return !tasks.empty() || is_stopped; });

					// Now lock is held
					if (is_stopped && tasks.empty()) return;

					// Obtain task and execute
					task = std::move(tasks.front());
					tasks.pop();
				}

				// Execute task
				task();
			}
		});
	}
}

thread_pool::~thread_pool() {
	{  // critical section
		auto lock = std::unique_lock<std::mutex>(q_mtx);
		is_stopped = true;
	}

	// notify all threads that the pool is stopped
	q_cv.notify_all();

	for (auto& t : threads) {
		t.join();  // end all threads
	}
}
};	// namespace ricox