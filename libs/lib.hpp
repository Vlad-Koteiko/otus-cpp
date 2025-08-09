#pragma once
#include "details.hpp"
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>

int version();

template <typename T,
          typename std::enable_if_t<std::is_integral<T>::value, int> = 0>
inline void print_ip(T value) {

  std::string str;

  for (std::size_t i = sizeof(T) - 1; i > 0; --i) {
    str += std::to_string(static_cast<std::uint8_t>(value >> (8 * i)));
    str += '.';
  }

  str += std::to_string(static_cast<std::uint8_t>(value));
  std::cout << str << std::endl;
}

template <typename T, typename std::enable_if_t<
                          std::is_same<T, std::string>::value, int> = 0>
inline void print_ip(T str) {

  std::cout << str << std::endl;
}

template <typename T,
          typename std::enable_if_t<details::is_container_v<T>, int> = 0>
inline void print_ip(const T &cont) {

  std::string str;

  for (auto v : cont) {

    str += std::to_string(v);
    str += '.';
  }

  str.pop_back();
  std::cout << str << std::endl;
}

template <typename T,
          typename std::enable_if_t<
              details::is_tuple_v<T> && details::tuple_fold_v<T>, int> = 0>
inline void print_ip(const T &typle) {

  std::string str;

  std::apply(
      [&str](const auto &...args) {
        ((str += std::to_string(args), str += '.'), ...);
      },
      typle);

  str.pop_back();
  std::cout << str << std::endl;
}
