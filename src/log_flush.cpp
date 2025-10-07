#include "log_flush.hpp"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <syncstream>

namespace fs = std::filesystem;

namespace ricox {

log_flush::~log_flush() = default;

auto std_flush::flush(const std::string& text) -> void { std::osyncstream{std::cout} << text; }

file_flush::file_flush(const std::string& file_name_) : file_name(file_name_) {
  auto path = fs::path(file_name_);
  if (!path.parent_path().empty()) fs::create_directories(path.parent_path());  // ensure the directory exists
  outfile.open(file_name_, std::ios::app);                                      // open file in append mode
  if (!outfile.is_open()) throw std::runtime_error("Failed to open log file: " + file_name);
}

file_flush::~file_flush() {
  if (outfile.is_open()) outfile.close();
}

auto file_flush::flush(const std::string& text) -> void { std::osyncstream{outfile} << text; }

roll_flush::roll_flush(const std::string& base_name, size_t max_size_)
    : base_name(base_name), max_size(max_size_), curr_size(0) {
  if (max_size_ == 0) throw std::invalid_argument("max_size must be greater than 0");

  auto path = fs::path(base_name);
  if (path.has_extension()) {
    // If the base name has an extension, remove it to avoid confusion
    this->base_name = path.replace_extension("").string();
  }

  if (!path.parent_path().empty()) fs::create_directories(path.parent_path());  // ensure the directory exists
}

roll_flush::roll_flush(const std::string& base_name) : roll_flush(base_name, DEFAULT_MAX_SIZE) {
  // This constructor uses the default max size
}

roll_flush::~roll_flush() {
  if (outfile.is_open()) outfile.close();  // close the file if it is open
}

auto roll_flush::flush(const std::string& text) -> void {
  static auto count = 0;  // static variable to keep track of the number of flush calls

  if (!outfile.is_open() || curr_size + text.size() > max_size) {
    if (outfile.is_open()) outfile.close();  // close the current file if it is open

    // generate a new file name with a timestamp
    auto ss = std::stringstream{};
    auto in_time_t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    ss << std::put_time(std::localtime(&in_time_t), "%I_%M_%S_%p");  // format the current time
    auto time_str = ss.str();

    auto new_file_name = base_name;
    new_file_name += "_" + time_str;                          // append the current time to the base name
    new_file_name += "_" + std::to_string(count++) + ".txt";  // append a count to avoid overwriting files

    outfile.open(new_file_name, std::ios::app);  // open a new file in append mode
    if (!outfile.is_open()) throw std::runtime_error("Failed to open log file: " + new_file_name);

    curr_size = 0;  // reset the current size
  }

  std::osyncstream{outfile} << text;
  curr_size += text.size();  // update the current size
}

}  // namespace ricox