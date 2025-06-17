#include "log_flush.hpp"

#include <iostream>

namespace ricox {

void std_flush::flush(const std::string& text) {
    std::cout << text;
}

void file_flush::flush(const std::string& text) {
    
}

void roll_flush::flush(const std::string& text) {}

}  // namespace ricox