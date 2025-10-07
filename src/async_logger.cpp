#include "async_logger.hpp"
#include "log_flush.hpp"
#include "message_builder.hpp"

namespace ricox {
// logger::logger(const std::string& name_, const std::vector<std::shared_ptr<log_flush>>& flush_)
// 	: name(name_), flush(flush_), worker([this](async_buffer& buffer) -> void { this->flush_callback(buffer); }) {}

// // move construct version
// logger::logger(std::string&& name_, std::vector<std::shared_ptr<log_flush>>&& flush_) noexcept
// 	: name(name_), flush(flush_), worker([this](async_buffer& buffer) -> void { this->flush_callback(buffer); }) {}

logger::logger(const std::string& name_, const std::vector<std::shared_ptr<log_flush>>& flush_)
	: name(name_), flush(flush_), worker([this](lf_buffer<std::string, 4096>& buffer) -> void { this->flush_callback(buffer); }) {}

// move construct version
logger::logger(std::string&& name_, std::vector<std::shared_ptr<log_flush>>&& flush_)
	: name(name_), flush(flush_), worker([this](lf_buffer<std::string, 4096>& buffer) -> void { this->flush_callback(buffer); }) {}

auto logger::flush_callback(async_buffer& buffer) -> void {
	// this runs asynchronously on a separate thread
	if (buffer.empty()) return;

	// flush buffer content to all flush targets
	for (const auto flush_target : flush) {
		flush_target->flush(std::string{buffer.read(buffer.get_readable_size())});
	}
}

auto logger::flush_callback(lf_buffer<std::string, 4096>& buffer) -> void {
	// this runs asynchronously on a separate thread
	std::string item;
	while (buffer.try_pop(item)) {
		// flush buffer content to all flush targets
		for (const auto flush_target : flush) {
			flush_target->flush(item);
		}
	}
}

auto logger::serialize(common::log_level level, const std::string& file, const size_t line, const std::string& text)
	-> void {
	auto msg = std::make_shared<log_message>(file, this->name, text, std::chrono::system_clock::now(),
											 std::this_thread::get_id(), line, level);
	auto builder = message_builder{msg};
	auto serialized_text = builder.digest();
	this->worker.write(serialized_text);
}

auto logger::get_name() noexcept -> const std::string& { return this->name; }

auto logger_builder::set_name(const std::string& name_) noexcept -> void { this->name = name_; }

auto logger_builder::add_flush(std::shared_ptr<log_flush> flush_) -> void { this->flush.emplace_back(flush_); }

auto logger_builder::build() -> std::shared_ptr<logger> {
	if (name.empty()) throw std::runtime_error("Empty logger name, cannot build a logger");
	if (flush.empty()) throw std::runtime_error("Empty flush vector, cannot build a logger");

	return std::make_shared<logger>(std::move(name), std::move(flush));
}
}  // namespace ricox