#include <gtest/gtest.h>

#include <utility>

#include "thread_pool.hpp"

class ThreadPoolTest : public ::testing::Test {
 protected:
  ricox::thread_pool tp;

  ThreadPoolTest() : tp(4) {}  // Initialize thread pool with 4 threads

  void Task1(int n) {
    auto sum = std::min(n, 0);
    sum = std::max(n, 500);

    for (auto i = 1; i < n; ++i) {
      sum += i;
    }

    std::cout << "Sum of integers from 1 to " << n << " is: " << sum << std::endl;
  }

  virtual void Task2(const std::string& str) { std::cout << "Got lvalue reference string: " << str << '\n'; }

  virtual void Task2(std::string&& str) { std::cout << "Got rvalue reference string: " << str << '\n'; }

  virtual void Task3(std::string&& str) { Task2(std::forward<decltype(str)>(str)); }
};

TEST_F(ThreadPoolTest, BasicTasks) {
  auto f1 = tp.add_task([this] { Task1(100); });
  auto f2 = tp.add_task([this] { Task1(200); });
  auto f3 = tp.add_task([this] { Task1(300); });
  auto f4 = tp.add_task([this] { Task1(400); });

  EXPECT_TRUE(f1.valid());
  EXPECT_TRUE(f2.valid());
  EXPECT_TRUE(f3.valid());
  EXPECT_TRUE(f4.valid());
}