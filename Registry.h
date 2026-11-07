#ifndef REGISTRY_HPP
#define REGISTRY_HPP

#include "Errors.h"
#include <array>
#include <cstddef>
#include <ostream>
#include <string>

template<typename T, int Cap>
class Registry {
  static_assert(Cap > 0, "Registry capacity must be positive");

public:
  void add(const T& value) {
    if (size_ >= static_cast<std::size_t>(Cap)) {
      throw InvalidArgument("registry is full");
    }
    data_[size_++] = value;
  }

  std::size_t size() const { return size_; }
  static constexpr std::size_t capacity() { return static_cast<std::size_t>(Cap); }

  T& operator[](std::size_t i) { return data_[i]; }
  const T& operator[](std::size_t i) const { return data_[i]; }

  T* begin() { return data_.data(); }
  T* end() { return data_.data() + size_; }
  const T* begin() const { return data_.data(); }
  const T* end()   const { return data_.data() + size_; }

  void printTo(std::ostream& os) const {
    os << "Registry[" << size_ << "/" << Cap << "]:";
    for (const auto& x : *this) os << ' ' << x;
  }

private:
  std::array<T, Cap> data_{};
  std::size_t size_ = 0;
};

template<int Cap>
class Registry<std::string, Cap> {
  static_assert(Cap > 0, "Registry capacity must be positive");

public:
  void add(const std::string& value) {
    if (size_ >= static_cast<std::size_t>(Cap)) {
      throw InvalidArgument("registry is full");
    }
    data_[size_++] = value;
  }

  std::size_t size() const { return size_; }
  static constexpr std::size_t capacity() { return static_cast<std::size_t>(Cap); }

  std::string& operator[](std::size_t i) { return data_[i]; }
  const std::string& operator[](std::size_t i) const { return data_[i]; }

  std::string* begin() { return data_.data(); }
  std::string* end() { return data_.data() + size_; }
  const std::string* begin() const { return data_.data(); }
  const std::string* end()   const { return data_.data() + size_; }

  void printTo(std::ostream& os) const {
    os << "Registry<string>[" << size_ << "/" << Cap << "]:";
    for (const auto& x : *this) os << " \"" << x << '"';
  }

private:
  std::array<std::string, Cap> data_{};
  std::size_t size_ = 0;
};

template<typename T, int Cap>
std::ostream& operator<<(std::ostream& os, const Registry<T, Cap>& r) {
  r.printTo(os);
  return os;
}

#endif