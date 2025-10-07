#include "logger.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <thread>

using namespace std::chrono_literals;

class LoggerTest : public testing::Test {
 protected:
  std::shared_ptr<ricox::logger> console_logger_;

  std::shared_ptr<ricox::logger> file_logger0_;
  std::shared_ptr<ricox::logger> file_logger1_;
  std::shared_ptr<ricox::logger> file_logger2_;
  std::shared_ptr<ricox::logger> file_logger3_;
  std::shared_ptr<ricox::logger> file_logger4_;

  LoggerTest()
      : console_logger_{ricox::common::create_logger("TEST_LOGGER_CONSOLE", {std::make_shared<ricox::std_flush>()})},
        file_logger0_{ricox::common::create_logger(
            "TEST_LOGGER_FILE0",
            {std::make_shared<ricox::file_flush>("/home/ricox/Documents/async_logger/test/file_log/test0.txt")})},
        file_logger1_{ricox::common::create_logger(
            "TEST_LOGGER_FILE1",
            {std::make_shared<ricox::file_flush>("/home/ricox/Documents/async_logger/test/file_log/test1.txt")})},
        file_logger2_{ricox::common::create_logger(
            "TEST_LOGGER_FILE2",
            {std::make_shared<ricox::file_flush>("/home/ricox/Documents/async_logger/test/file_log/test2.txt")})},
        file_logger3_{ricox::common::create_logger(
            "TEST_LOGGER_FILE3",
            {std::make_shared<ricox::file_flush>("/home/ricox/Documents/async_logger/test/file_log/test3.txt")})},
        file_logger4_{ricox::common::create_logger(
            "TEST_LOGGER_FILE4",
            {std::make_shared<ricox::file_flush>("/home/ricox/Documents/async_logger/test/file_log/test4.txt")})} {}
};

TEST(Init, Cleanup) { std::system("rm /home/ricox/Documents/async_logger/test/file_log/test*.txt"); }

TEST_F(LoggerTest, ConsoleLogging) {
  ASSERT_NE(console_logger_, nullptr);
  console_logger_->info(std::source_location::current(), "This is an info message: {}", 42);
  console_logger_->debug(std::source_location::current(), "This is a debug message: {}", 3.14);
  console_logger_->warn(std::source_location::current(), "This is a warning message");
  console_logger_->error(std::source_location::current(), "This is an error message");
  console_logger_->fatal(std::source_location::current(), "This is a fatal message");
}

TEST_F(LoggerTest, FileLogging) {
  ASSERT_NE(file_logger0_, nullptr);
  file_logger0_->info(std::source_location::current(), "File info message: {}", "Hello, File!");
  file_logger0_->debug(std::source_location::current(), "File debug message: {}", 12345);
  file_logger0_->warn(std::source_location::current(), "File warning message");
  file_logger0_->error(std::source_location::current(), "File error message");
  file_logger0_->fatal(std::source_location::current(), "File fatal message");
}

TEST_F(LoggerTest, ThroughputInt32) {
  ASSERT_NE(file_logger0_, nullptr);
  ASSERT_NE(console_logger_, nullptr);

  auto start_time = std::chrono::steady_clock::now();
  while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time) <
         1000ms) {
    file_logger0_->info(std::source_location::current(), "{}", 5555555);
  }
}

TEST_F(LoggerTest, ThroughputStr10) {
  ASSERT_NE(file_logger1_, nullptr);
  ASSERT_NE(console_logger_, nullptr);
  constexpr const char msg[] = "0123 0123";  // 32 bytes

  auto start_time = std::chrono::steady_clock::now();
  while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time) <
         1000ms) {
    file_logger1_->info(std::source_location::current(), "{}", msg);
  }
}

TEST_F(LoggerTest, ThroughputStr32) {
  ASSERT_NE(file_logger2_, nullptr);
  ASSERT_NE(console_logger_, nullptr);
  constexpr const char msg[] = "0123 0123 0123 0123 0123 0123 1";  // 32 bytes

  auto start_time = std::chrono::steady_clock::now();
  while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time) <
         1000ms) {
    file_logger2_->info(std::source_location::current(), "{}", msg);
  }
}

TEST(End, Report) {
  std::system("du -h /home/ricox/Documents/async_logger/test/file_log/*.txt");
}