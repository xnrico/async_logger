#include "common.hpp"

namespace ricox {
namespace common {
auto get_logger(const std::string& name) -> std::shared_ptr<logger> { return manager::get_instance().get_logger(name); }

auto get_default_logger() -> std::shared_ptr<logger> { return manager::get_instance().get_default_logger(); }
}  // namespace common
}  // namespace ricox