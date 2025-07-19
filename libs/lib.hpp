#pragma once
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

using Ip = std::tuple<std::uint8_t, std::uint8_t, std::uint8_t, std::uint8_t>;

using FormatString = std::tuple<Ip, std::string, std::string>;

using Pool = std::vector<FormatString>;

int version();

[[nodiscard]] Ip getIp(const std::string &str);
[[nodiscard]] FormatString getFormatString(const std::string &str);
void debugInfo(const Pool &pool);
void sort(Pool &ip_pool);
[[nodiscard]] Pool filter(const Pool &pool, std::uint8_t value);
[[nodiscard]] Pool filter(const Pool &pool, std::uint8_t valueFerst);
[[nodiscard]] Pool filter(const Pool &pool, std::uint8_t valueFerst,
                          std::uint8_t valueSecond);
[[nodiscard]] Pool filter_any(const Pool &pool, std::uint8_t valueFerst);
