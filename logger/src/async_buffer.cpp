#include "async_buffer.hpp"
#include "common.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace ricox {

async_buffer::async_buffer() : producer_idx(0U), consumer_idx(0U) { this->buffer.resize(common::DEFAULT_BUFFER_SIZE); }

async_buffer::async_buffer(size_t buffer_size) : producer_idx(0U), consumer_idx(0U) { this->buffer.resize(buffer_size); }

auto async_buffer::get_readable_size() -> size_t {
	if (this->consumer_idx > this->producer_idx) throw std::runtime_error("Invalid consumer index");
	return this->producer_idx - this->consumer_idx;
}

auto async_buffer::get_writable_size() noexcept -> size_t { return this->buffer.size() - this->producer_idx; }

auto async_buffer::advance_producer(size_t step) -> void {
	if (step > this->get_writable_size()) throw std::runtime_error("No buffer space available");
	this->producer_idx += step;
}

auto async_buffer::advance_consumer(size_t step) -> void {
	if (step > this->get_readable_size()) throw std::runtime_error("Invalid advance by consumer");
	this->consumer_idx += step;
}

auto async_buffer::reset() noexcept -> void {
	this->producer_idx = 0U;
	this->consumer_idx = 0U;
}

auto async_buffer::swap(async_buffer& other) noexcept -> void {
	std::swap(this->buffer, other.buffer);
	std::swap(this->producer_idx, other.producer_idx);
	std::swap(this->consumer_idx, other.consumer_idx);
}

auto async_buffer::empty() noexcept -> bool { return producer_idx == consumer_idx; }

auto async_buffer::read(size_t len) -> std::string_view {
	if (len > this->get_readable_size()) throw std::runtime_error("Invalid read by consumer");
	return std::string_view(this->buffer.data() + consumer_idx, len);
}

auto async_buffer::write(std::string_view sv) -> void {
	if (sv.size() > this->get_writable_size()) throw std::runtime_error("Invalid write by producer");
	std::ranges::copy(sv, this->buffer.data() + producer_idx);
}
};	// namespace ricox