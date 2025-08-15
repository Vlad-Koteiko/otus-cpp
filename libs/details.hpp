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

template <typename Tuple, typename = void>
struct tuple_fold : std::false_type {};

template <typename... Ts>
struct tuple_fold<
    std::tuple<Ts...>,
    std::void_t<std::enable_if_t<(std::is_same_v<Ts, std::tuple_element_t<0, std::tuple<Ts...>>> && ...)>>
> : std::true_type {};

template <typename Tuple>
constexpr bool tuple_fold_v = tuple_fold<Tuple>::value;

template <typename T> struct is_tuple : std::false_type {};

template <typename... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type {};

template <typename T> inline constexpr bool is_tuple_v = is_tuple<T>::value;

}; // namespace details
