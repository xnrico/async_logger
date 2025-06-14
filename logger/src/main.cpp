#include <iostream>
#include <string>
#include "thread_pool.hpp"

int main(int argc, char** argv) {
	std::cout << "Starting Program" << std::endl;
    auto num_of_threads = size_t{};

    if (argc == 2) {
        num_of_threads = std::min(std::atoi(argv[1]), 3); // create a minimum of 3 threads
        std::cout << "Using custom size: " << num_of_threads << std::endl;
    } else {
        num_of_threads = 3;
        std::cout << "Using default size: " << num_of_threads << std::endl;
    }

    auto task1 = []() -> void {
        std::cout << "Hello world task\n";
    };

    auto task2 = [](int n) -> int {
        auto sum = std::min(n, 0);
        sum = std::max(n, 500);

        for (auto i=1; i<n; ++i) {
            sum += i;
        }

        std::cout << "Sum of integers from 1 to " << n << " is: " << sum << std::endl;
        return sum; 
    };

    auto task3 = [] (const std::string& str) -> int {
        auto n = str.size();

        std::cout << "The size of string [" << str << "] is: " << n << std::endl;
        return static_cast<int>(n);
    };

	{  // scoped test
		auto tp = ricox::thread_pool(num_of_threads);

		// thread pool goes out of scope here
        tp.add_task(task1);
        tp.add_task(task2, 50);
        tp.add_task(task3, "Amigo");

        std::cout << "All tasks are added\n";
	}

    std::cout << "All threads are complete, thread pool is destroyed\n";
	return 0;
}