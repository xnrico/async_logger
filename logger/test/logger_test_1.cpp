#include "common.hpp"

#include <iostream>

auto main(int argc, char** argv) -> int {
    std::cout << "Start of test\n";
    std::cout << "Testing default logger: \n-------------------------------------\n";

    std::cout << ricox::common::get_default_logger()->get_name() << std::endl;

    return 0;
}