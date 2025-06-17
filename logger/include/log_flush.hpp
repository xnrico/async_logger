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
	virtual void flush(const std::string& text) = 0;
};

class std_flush final : public log_flush {
   public:
	void flush(const std::string& text) override;
};

class file_flush final : public log_flush {
   private:
	std::string file_name;
	std::ofstream outfile;

   public:
	void flush(const std::string& text) override;
};

class roll_flush final : public log_flush {
   private:
	std::string base_name;
	std::ofstream outfile;
	size_t max_size;
	size_t curr_size;

   public:
	void flush(const std::string& text) override;
};

class log_flush_factory {
   public:
	template <typename flush_type, typename... Args>
	std::shared_ptr<log_flush> create(Args&&... args) {
		return std::make_shared<flush_type>(std::forward<Args>(args))...;
	}
};
}  // namespace ricox