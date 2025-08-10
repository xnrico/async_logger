#pragma once

#include <fstream>
#include <memory>
#include <string>

// using factory design pattern to generate log_flush object that flush output to destinations below:
// 1. std::cout
// 2. fixed file
// 3. rolling files with same base name

namespace ricox {
class log_flush {
   public:
	virtual auto flush(const std::string& text) -> void = 0;
	virtual ~log_flush() = 0;  // for inheritence
};

class std_flush final : public log_flush {
   public:
	auto flush(const std::string& text) -> void override;
};

class file_flush final : public log_flush {
   private:
	std::string file_name;
	std::ofstream outfile;

   public:
	file_flush() = delete;
	file_flush(const std::string& file_name_);
	~file_flush() override;
	auto flush(const std::string& text) -> void override;
};

class roll_flush final : public log_flush {
   private:
	static constexpr size_t DEFAULT_MAX_SIZE = 2 << 11;  // default max size is 4 MB

	std::string base_name;
	std::ofstream outfile;
	size_t max_size;
	size_t curr_size;

   public:
	roll_flush() = delete;
	roll_flush(const std::string& base_name);
	roll_flush(const std::string& base_name, size_t max_size_);
	~roll_flush() override;
	auto flush(const std::string& text) -> void override;
};

class log_flush_factory {
   public:
	template <typename flush_type, typename... Args>
	auto create(Args&&... args) -> std::shared_ptr<log_flush> {
		return std::make_shared<flush_type>(std::forward<Args>(args)...);
	}
};
}  // namespace ricox