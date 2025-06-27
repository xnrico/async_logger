#include "common.hpp"

#include <chrono>
#include <iostream>
#include <thread>

auto log_info(unsigned int freq) -> void {
	for (auto i = 0; i < freq; ++i) {
		ricox::common::get_default_logger()->log(ricox::common::log_level::INFO, __FILE__, __LINE__,
												 "This is a test log message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

auto log_debug(unsigned int freq) -> void {
	for (auto i = 0; i < freq; ++i) {
		ricox::common::get_default_logger()->log(ricox::common::log_level::DEBUG, __FILE__, __LINE__,
												 "This is a test debug message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

auto log_error(unsigned int freq) -> void {
	for (auto i = 0; i < freq; ++i) {
		ricox::common::get_default_logger()->log(ricox::common::log_level::ERROR, __FILE__, __LINE__,
												 "This is a test error message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

auto log_warning(unsigned int freq) -> void {
	for (auto i = 0; i < freq; ++i) {
		ricox::common::get_default_logger()->log(ricox::common::log_level::WARN, __FILE__, __LINE__,
												 "This is a test warning message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

auto log_fatal(unsigned int freq) -> void {
	for (auto i = 0; i < freq; ++i) {
		ricox::common::get_default_logger()->log(ricox::common::log_level::FATAL, __FILE__, __LINE__,
												 "This is a test critical message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

auto main(int argc, char** argv) -> int {
	std::cout << "Start of test\n";
	std::cout << "Testing default info logger: \n-------------------------------------\n";

	std::cout << ricox::common::get_default_logger()->get_name() << std::endl << std::endl;

	auto t1 = std::thread(log_info, 10);
	auto t2 = std::thread(log_debug, 10);
	auto t3 = std::thread(log_error, 10);
	auto t4 = std::thread(log_warning, 10);
	auto t5 = std::thread(log_fatal, 10);

	for (auto i = 0; i < 10; ++i) {
		ricox::common::get_default_logger()->log(ricox::common::log_level::INFO, __FILE__, __LINE__,
												 "MAIN THREAD: This is a test log message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	std::cout << "End of test\n";
	std::cout << "-------------------------------------\n";
	std::cout << "All messages should be flushed to the console and files.\n";

	return 0;
}