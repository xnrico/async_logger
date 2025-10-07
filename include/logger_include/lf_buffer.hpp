#pragma once

#include <array>
#include <atomic>

// lock-free circular buffer for single producer and single consumer
template <typename T, size_t Size>
// Size must be a power of 2
class lf_buffer {
  static_assert((Size & (Size - 1)) == 0, "Size must be a power of 2");
  std::array<T, Size> buffer_;
  alignas(64) std::atomic<size_t> write_pos_{0};
  alignas(64) std::atomic<size_t> read_pos_{0};

 public:
  bool try_push(T&& item) {
    size_t write = write_pos_.load(std::memory_order_relaxed);
    size_t next = (write + 1) & (Size - 1);  // optimization for modulo operation when Size is power of 2

    if (next == read_pos_.load(std::memory_order_acquire)) {
      return false;  // Full
    }

    buffer_[write] = std::move(item);
    write_pos_.store(next, std::memory_order_release);
    return true;
  }

  bool try_pop(T& item) {
    size_t read = read_pos_.load(std::memory_order_relaxed);

    if (read == write_pos_.load(std::memory_order_acquire)) {
      return false;  // Empty
    }

    item = std::move(buffer_[read]);
    read_pos_.store((read + 1) & (Size - 1), std::memory_order_release);
    return true;
  }

  bool empty() const { return read_pos_.load(std::memory_order_acquire) == write_pos_.load(std::memory_order_acquire); }

  bool full() const {
    return ((write_pos_.load(std::memory_order_acquire) + 1) & (Size - 1)) == read_pos_.load(std::memory_order_acquire);
  }
};  