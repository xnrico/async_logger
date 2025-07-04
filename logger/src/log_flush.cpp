#include "log_flush.hpp"

#include <iostream>

namespace ricox {

log_flush::~log_flush() = default;

auto std_flush::flush(const std::string& text) -> void { std::cout << text; }

auto file_flush::flush(const std::string& text) -> void {}

auto roll_flush::flush(const std::string& text) -> void {}

}  // namespace ricox