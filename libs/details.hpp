#pragma once
#include <cassert>
#include <list>
#include <type_traits>
#include <vector>

namespace details {

template <typename T> struct is_container : std::false_type {};

template <typename T, typename Alloc>
struct is_container<std::vector<T, Alloc>> : std::true_type {};

template <typename T, typename Alloc>
struct is_container<std::list<T, Alloc>> : std::true_type {};

template <typename T>
inline constexpr bool is_container_v = is_container<T>::value;

template <typename... Ts> constexpr bool types_fold() {
  if constexpr (sizeof...(Ts) == 0) {
    return true;
  } else {
    return (std::is_same_v<std::tuple_element_t<0, std::tuple<Ts...>>, Ts> && ...);
  }
}

template <typename Tuple> struct tuple_fold : std::false_type {};

template <typename... Ts>
struct tuple_fold<std::tuple<Ts...>> : std::bool_constant<types_fold<Ts...>()> {};

template <typename Tuple>
constexpr bool tuple_fold_v = tuple_fold<Tuple>::value;

template <typename T> struct is_tuple : std::false_type {};

template <typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

template <typename T> inline constexpr bool is_tuple_v = is_tuple<T>::value;

}; // namespace details
