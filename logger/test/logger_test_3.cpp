#include "common.hpp"
#include "log_flush.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <source_location>

auto log_info(unsigned int freq, std::shared_ptr<ricox::logger> logger_) -> void {
	auto log = logger_ ? logger_ : ricox::common::get_default_logger();

	for (auto i = 0; i < freq; ++i) {
		log->log(ricox::common::log_level::DEBUG, std::source_location::current(), "This is a test log message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

auto main(int argc, char** argv) -> int {
	std::cout << "Start of test\n";

	auto log1 = ricox::common::create_logger(
		"LOGGER1",
		{std::make_shared<ricox::file_flush>("/home/ricox/Documents/async_logger/logger/test/file_log/log1.txt")});
	auto log2 = ricox::common::create_logger<ricox::file_flush>(
		"LOGGER2", "/home/ricox/Documents/async_logger/logger/test/file_log/log2.txt");
	auto log3 = ricox::common::create_logger(
		"LOGGER3",
		{std::make_shared<ricox::file_flush>("/home/ricox/Documents/async_logger/logger/test/file_log/log3.txt")});

	auto t0 = std::thread(log_info, 10, nullptr);
	auto t1 = std::thread(log_info, 10, log1);
	auto t2 = std::thread(log_info, 10, log2);
	auto t3 = std::thread(log_info, 10, log3);

	for (auto i = 0; i < 10; ++i) {
		ricox::common::INFO_DEFAULT("This is a test info MAIN message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	t0.join();
	t1.join();
	t2.join();
	t3.join();

	std::cout << "End of test\n";
	std::cout << "-------------------------------------\n";
	std::cout << "All messages should be flushed to the console and files.\n";

	return 0;
}