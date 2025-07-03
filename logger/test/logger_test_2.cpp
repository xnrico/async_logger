#include "common.hpp"
#include "log_flush.hpp"

#include <chrono>
#include <iostream>
#include <thread>

auto log_info(unsigned int freq, std::shared_ptr<ricox::logger> logger_) -> void {
	auto log = logger_ ? logger_ : ricox::common::get_default_logger();

	for (auto i = 0; i < freq; ++i) {
		log->log(ricox::common::log_level::DEBUG, __FILE__, __LINE__, "This is a test log message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

auto main(int argc, char** argv) -> int {
	std::cout << "Start of test\n";
	std::cout << "Testing default info logger: \n-------------------------------------\n";

	std::cout << ricox::common::get_default_logger()->get_name() << std::endl << std::endl;

	auto log1 = ricox::common::create_logger("LOGGER1", {std::make_shared<ricox::std_flush>()});
	auto log2 = ricox::common::create_logger("LOGGER2", {std::make_shared<ricox::std_flush>()});
	auto log3 = ricox::common::create_logger("LOGGER3", {std::make_shared<ricox::std_flush>()});

    auto t0 = std::thread(log_info, 10, nullptr);
    auto t1 = std::thread(log_info, 10, log1);
    auto t2 = std::thread(log_info, 10, log2);
    auto t3 = std::thread(log_info, 10, log3);

	for (auto i = 0; i < 10; ++i) {
		ricox::common::INFO_DEFAULT("This is a test info MAINN message.");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	t1.join();
	t2.join();
	t3.join();


	std::cout << "End of test\n";
	std::cout << "-------------------------------------\n";
	std::cout << "All messages should be flushed to the console and files.\n";

	return 0;
}