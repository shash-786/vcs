#pragma once

#include <stdexcept>
#include <variant>
template <typename T, typename E> class Result {
private:
  std::variant<T, E> data;

public:
  Result(const T &v) : data(v) {}
  Result(const E &e) : data(e) {}

  bool err_present() { return std::holds_alternative<E>(data); }

  bool success() { return std::holds_alternative<T>(data); }

  E error() {
    if (success()) {
      std::runtime_error("Accessing error from a Success result");
    }
    return std::get<E>(data);
  }

  T value() {
    if (err_present()) {
      std::runtime_error("Accessing value from a Error result");
    }
    return std::get<T>(data);
  }
};
