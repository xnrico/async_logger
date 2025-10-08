#include "logger.hpp"

#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <thread>
#include <filesystem>

using namespace std::chrono_literals;

class MultiThreadLoggerTest : public testing::Test
{
protected:
    std::string curr_path;

    std::shared_ptr<ricox::logger> console_logger_;

    std::shared_ptr<ricox::logger> file_logger0_;
    std::shared_ptr<ricox::logger> file_logger1_;
    std::shared_ptr<ricox::logger> file_logger2_;
    std::shared_ptr<ricox::logger> file_logger3_;
    std::shared_ptr<ricox::logger> file_logger4_;
    std::shared_ptr<ricox::logger> file_logger5_;
    std::shared_ptr<ricox::logger> file_logger6_;
    std::shared_ptr<ricox::logger> file_logger7_;
    std::shared_ptr<ricox::logger> file_logger8_;
    std::shared_ptr<ricox::logger> file_logger9_;

    MultiThreadLoggerTest()
        : curr_path{std::filesystem::path(__FILE__).remove_filename()},
          console_logger_{ricox::common::create_logger("TEST_LOGGER_CONSOLE", {std::make_shared<ricox::std_flush>()})},
          file_logger0_{ricox::common::create_logger(
              "TEST_LOGGER_FILE0",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult0.txt")})},
          file_logger1_{ricox::common::create_logger(
              "TEST_LOGGER_FILE1",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult1.txt")})},
          file_logger2_{ricox::common::create_logger(
              "TEST_LOGGER_FILE2",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult2.txt")})},
          file_logger3_{ricox::common::create_logger(
              "TEST_LOGGER_FILE3",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult3.txt")})},
          file_logger4_{ricox::common::create_logger(
              "TEST_LOGGER_FILE4",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult4.txt")})},
          file_logger5_{ricox::common::create_logger(
              "TEST_LOGGER_FILE5",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult5.txt")})},
          file_logger6_{ricox::common::create_logger(
              "TEST_LOGGER_FILE6",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult6.txt")})},
          file_logger7_{ricox::common::create_logger(
              "TEST_LOGGER_FILE7",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult7.txt")})},
          file_logger8_{ricox::common::create_logger(
              "TEST_LOGGER_FILE8",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult8.txt")})},
          file_logger9_{ricox::common::create_logger(
              "TEST_LOGGER_FILE9",
              {std::make_shared<ricox::file_flush>(curr_path + "file_log/mult9.txt")})} {}
};

TEST(Init, Report)
{
    auto path = std::filesystem::path(__FILE__).remove_filename().string();
    auto _  = std::system((std::string{"rm "} + path + "file_log/mult*.txt").c_str());
}

TEST_F(MultiThreadLoggerTest, MultiThreadThroughputStr8)
{
    ASSERT_NE(console_logger_, nullptr);
    ASSERT_NE(file_logger0_, nullptr);
    ASSERT_NE(file_logger1_, nullptr);
    ASSERT_NE(file_logger2_, nullptr);
    ASSERT_NE(file_logger3_, nullptr);
    ASSERT_NE(file_logger4_, nullptr);
    ASSERT_NE(file_logger5_, nullptr);
    ASSERT_NE(file_logger6_, nullptr);
    ASSERT_NE(file_logger7_, nullptr);
    ASSERT_NE(file_logger8_, nullptr);
    ASSERT_NE(file_logger9_, nullptr);

    constexpr const char msg8[] = "0123 0123"; // 8 bytes

    constexpr const char msg32[] =
        "0123 0123 0123 0123 0123 0123 1"; // 32 bytes

    constexpr const char msg128[] =
        "0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 "
        "0123 0123 0123 01"; // 128 bytes

    auto log_task = [this](std::shared_ptr<ricox::logger> logger, const char *msg) -> void
    {
    constexpr const char msg_local[] = "0123 0123 0123 0123 0123 0123 1"; // 32 bytes
    auto start_time = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time) <
           1000ms)
    {
      logger->info(std::source_location::current(), "{}", msg_local);
    }
    console_logger_->info(std::source_location::current(), "THREAD complete"); };

    auto thread0 = std::thread(log_task, file_logger0_, msg8);
    auto thread1 = std::thread(log_task, file_logger1_, msg8);
    auto thread2 = std::thread(log_task, file_logger2_, msg8);
    auto thread3 = std::thread(log_task, file_logger3_, msg8);
    auto thread4 = std::thread(log_task, file_logger4_, msg8);
    auto thread5 = std::thread(log_task, file_logger5_, msg8);
    auto thread6 = std::thread(log_task, file_logger6_, msg8);
    auto thread7 = std::thread(log_task, file_logger7_, msg8);
    auto thread8 = std::thread(log_task, file_logger8_, msg8);
    auto thread9 = std::thread(log_task, file_logger9_, msg8);

    thread0.join();
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    thread9.join();

    auto path = std::filesystem::path(__FILE__).remove_filename().string();
    auto _ = std::system((std::string{"du -h "} + path + "file_log/mult*.txt").c_str());
}

TEST_F(MultiThreadLoggerTest, MultiThreadThroughputStr32)
{
    ASSERT_NE(console_logger_, nullptr);
    ASSERT_NE(file_logger0_, nullptr);
    ASSERT_NE(file_logger1_, nullptr);
    ASSERT_NE(file_logger2_, nullptr);
    ASSERT_NE(file_logger3_, nullptr);
    ASSERT_NE(file_logger4_, nullptr);
    ASSERT_NE(file_logger5_, nullptr);
    ASSERT_NE(file_logger6_, nullptr);
    ASSERT_NE(file_logger7_, nullptr);
    ASSERT_NE(file_logger8_, nullptr);
    ASSERT_NE(file_logger9_, nullptr);

    constexpr const char msg8[] = "0123 0123"; // 8 bytes

    constexpr const char msg32[] =
        "0123 0123 0123 0123 0123 0123 1"; // 32 bytes

    constexpr const char msg128[] =
        "0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 "
        "0123 0123 0123 01"; // 128 bytes

    auto log_task = [this](std::shared_ptr<ricox::logger> logger, const char *msg) -> void
    {
    constexpr const char msg_local[] = "0123 0123 0123 0123 0123 0123 1"; // 32 bytes
    auto start_time = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time) <
           1000ms)
    {
      logger->info(std::source_location::current(), "{}", msg_local);
    }
    console_logger_->info(std::source_location::current(), "THREAD complete"); };

    auto thread0 = std::thread(log_task, file_logger0_, msg32);
    auto thread1 = std::thread(log_task, file_logger1_, msg32);
    auto thread2 = std::thread(log_task, file_logger2_, msg32);
    auto thread3 = std::thread(log_task, file_logger3_, msg32);
    auto thread4 = std::thread(log_task, file_logger4_, msg32);
    auto thread5 = std::thread(log_task, file_logger5_, msg32);
    auto thread6 = std::thread(log_task, file_logger6_, msg32);
    auto thread7 = std::thread(log_task, file_logger7_, msg32);
    auto thread8 = std::thread(log_task, file_logger8_, msg32);
    auto thread9 = std::thread(log_task, file_logger9_, msg32);

    thread0.join();
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    thread9.join();

    auto path = std::filesystem::path(__FILE__).remove_filename().string();
    auto _ = std::system((std::string{"du -h "} + path + "file_log/mult*.txt").c_str());
}

TEST_F(MultiThreadLoggerTest, MultiThreadThroughputStr128)
{
    ASSERT_NE(console_logger_, nullptr);
    ASSERT_NE(file_logger0_, nullptr);
    ASSERT_NE(file_logger1_, nullptr);
    ASSERT_NE(file_logger2_, nullptr);
    ASSERT_NE(file_logger3_, nullptr);
    ASSERT_NE(file_logger4_, nullptr);
    ASSERT_NE(file_logger5_, nullptr);
    ASSERT_NE(file_logger6_, nullptr);
    ASSERT_NE(file_logger7_, nullptr);
    ASSERT_NE(file_logger8_, nullptr);
    ASSERT_NE(file_logger9_, nullptr);

    constexpr const char msg8[] = "0123 0123"; // 8 bytes

    constexpr const char msg32[] =
        "0123 0123 0123 0123 0123 0123 1"; // 32 bytes

    constexpr const char msg128[] =
        "0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 "
        "0123 0123 0123 01"; // 128 bytes

    auto log_task = [this](std::shared_ptr<ricox::logger> logger, const char *msg) -> void
    {
    constexpr const char msg_local[] = "0123 0123 0123 0123 0123 0123 1"; // 32 bytes
    auto start_time = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time) <
           1000ms)
    {
      logger->info(std::source_location::current(), "{}", msg_local);
    }
    console_logger_->info(std::source_location::current(), "THREAD complete"); };

    auto thread0 = std::thread(log_task, file_logger0_, msg128);
    auto thread1 = std::thread(log_task, file_logger1_, msg128);
    auto thread2 = std::thread(log_task, file_logger2_, msg128);
    auto thread3 = std::thread(log_task, file_logger3_, msg128);
    auto thread4 = std::thread(log_task, file_logger4_, msg128);
    auto thread5 = std::thread(log_task, file_logger5_, msg128);
    auto thread6 = std::thread(log_task, file_logger6_, msg128);
    auto thread7 = std::thread(log_task, file_logger7_, msg128);
    auto thread8 = std::thread(log_task, file_logger8_, msg128);
    auto thread9 = std::thread(log_task, file_logger9_, msg128);

    thread0.join();
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    thread9.join();

    auto path = std::filesystem::path(__FILE__).remove_filename().string();
    auto _ = std::system((std::string{"du -h "} + path + "file_log/mult*.txt").c_str());
}

TEST_F(MultiThreadLoggerTest, MultiThreadThroughputStrMixed)
{
    ASSERT_NE(console_logger_, nullptr);
    ASSERT_NE(file_logger0_, nullptr);
    ASSERT_NE(file_logger1_, nullptr);
    ASSERT_NE(file_logger2_, nullptr);
    ASSERT_NE(file_logger3_, nullptr);
    ASSERT_NE(file_logger4_, nullptr);
    ASSERT_NE(file_logger5_, nullptr);
    ASSERT_NE(file_logger6_, nullptr);
    ASSERT_NE(file_logger7_, nullptr);
    ASSERT_NE(file_logger8_, nullptr);
    ASSERT_NE(file_logger9_, nullptr);

    constexpr const char msg8[] = "0123 0123"; // 8 bytes

    constexpr const char msg32[] =
        "0123 0123 0123 0123 0123 0123 1"; // 32 bytes

    constexpr const char msg128[] =
        "0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 0123 "
        "0123 0123 0123 01"; // 128 bytes

    auto log_task = [this](std::shared_ptr<ricox::logger> logger, const char *msg) -> void
    {
    constexpr const char msg_local[] = "0123 0123 0123 0123 0123 0123 1"; // 32 bytes
    auto start_time = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time) <
           1000ms)
    {
      logger->info(std::source_location::current(), "{}", msg_local);
    }
    console_logger_->info(std::source_location::current(), "THREAD complete"); };

    auto thread0 = std::thread(log_task, file_logger0_, msg8);
    auto thread1 = std::thread(log_task, file_logger1_, msg32);
    auto thread2 = std::thread(log_task, file_logger2_, msg128);
    auto thread3 = std::thread(log_task, file_logger3_, msg8);
    auto thread4 = std::thread(log_task, file_logger4_, msg32);
    auto thread5 = std::thread(log_task, file_logger5_, msg128);
    auto thread6 = std::thread(log_task, file_logger6_, msg8);
    auto thread7 = std::thread(log_task, file_logger7_, msg32);
    auto thread8 = std::thread(log_task, file_logger8_, msg128);
    auto thread9 = std::thread(log_task, file_logger9_, msg8);

    thread0.join();
    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    thread9.join();

    auto path = std::filesystem::path(__FILE__).remove_filename().string();
    auto _ = std::system((std::string{"du -h "} + path + "file_log/mult*.txt").c_str());
}