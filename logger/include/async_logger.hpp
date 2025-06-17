#pragma once

#include "log_flush.hpp"

#include <memory>
#include <vector>
#include <string>


namespace ricox {
class logger {
private:
    std::string name;
    std::vector<std::shared_ptr<log_flush>> flush;

public:
    
};

class logger_builder {
private:
    std::shared_ptr<logger> my_logger;

public:
    logger_builder();
    ~logger_builder() = default;

    logger_builder(const logger_builder&) = delete;
    logger_builder& operator=(const logger_builder&) = delete;
    logger_builder(logger_builder&&) = delete;
    logger_builder&& operator=(logger_builder&&) = delete;

    std::shared_ptr<logger> build();
};


}  // namespace ricox