#pragma once
#include "async_logger.hpp"

#include <unordered_map>
#include <string>

namespace ricox {
class manager { // manager will be a singleton class
private:
    std::unordered_map<std::string, std::shared_ptr<logger>> logger_map;

    manager();
    ~manager() = default;
    
    manager(const manager&) = delete;
    manager& operator=(const manager&) = delete;
    manager(manager&&) = delete;
    manager&& operator=(manager&&) = delete;

public:
    static manager& get_instance();
    std::shared_ptr<logger> get_logger(const std::string&);

};
}  // namespace ricox