#pragma once

#include <string>
#include <string_view>

namespace ricox {
class async_buffer {
 protected:
  std::string buffer;
  size_t producer_idx;  // writing
  size_t consumer_idx;  // reading

 public:
  async_buffer();
  async_buffer(size_t buffer_size);
  ~async_buffer() = default;

  auto get_readable_size() -> size_t;
  auto get_writable_size() noexcept -> size_t;
  auto advance_producer(size_t step) -> void;
  auto advance_consumer(size_t step) -> void;
  auto swap(async_buffer& other) noexcept -> void;
  auto reset() noexcept -> void;
  auto empty() noexcept -> bool;

  auto read(size_t len) -> std::string_view;
  auto write(std::string_view sv) -> void;
};
};  // namespace ricox