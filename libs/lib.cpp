#include "lib.hpp"
#include "version.h"

int version() { return PROJECT_VERSION_PATCH; }

[[nodiscard]] Ip getIp(const std::string &str) {

  std::vector<std::uint8_t> ip_pull;
  const char split = '.';

  std::string::size_type start = 0;
  std::string::size_type stop = str.find_first_of(split);

  for (std::size_t i = 0; i < 4; ++i) {

    // std::cout << "#" << std::stoi(str.substr(start, stop - start)) <<
    // std::endl;
    ip_pull.push_back(std::stoi(str.substr(start, stop - start)));
    start = stop + 1;
    stop = str.find_first_of(split, start);
  }

  return std::make_tuple(ip_pull[0], ip_pull[1], ip_pull[2], ip_pull[3]);
}

[[nodiscard]] FormatString getFormatString(const std::string &str) {

  std::vector<std::string> pull;
  const char split = '\t';

  std::string::size_type start = 0;
  std::string::size_type stop = str.find_first_of(split);

  for (std::size_t i = 0; i < 3; ++i) {

    // std::cout << "!" << str.substr(start, stop - start) << std::endl;
    pull.push_back(str.substr(start, stop - start));
    start = stop + 1;
    stop = str.find_first_of(split, start);
  }

  return std::make_tuple(getIp(pull[0]), pull[1], pull[2]);
}

void debugInfo(const Pool &pool) {

  for (auto &&v : pool) {

    auto &&[ip_tuple, text_1, text_2] = v;
    auto &&[ip_0, ip_1, ip_2, ip_3] = ip_tuple;

    std::cout << static_cast<int>(ip_0) << "." << static_cast<int>(ip_1) << "."
              << static_cast<int>(ip_2) << "." << static_cast<int>(ip_3)
              << "\t " << text_1 << "\t " << text_2 << std::endl;
  }
}

void sort(Pool &ip_pool) {

  auto sortCompor = [](const FormatString &a, const FormatString &b) {
    auto &&[ip_tuple_a, text_1_a, text_2_a] = a;
    auto &&[ip_0_a, ip_1_a, ip_2_a, ip_3_a] = ip_tuple_a;
    auto &&[ip_tuple_b, text_1_b, text_2_b] = b;
    auto &&[ip_0_b, ip_1_b, ip_2_b, ip_3_b] = ip_tuple_b;

    if (ip_0_a != ip_0_b)
      return ip_0_a > ip_0_b;
    if (ip_1_a != ip_1_b)
      return ip_1_a > ip_1_b;
    if (ip_2_a != ip_2_b)
      return ip_2_a > ip_2_b;
    return ip_3_a > ip_3_b;
  };

  std::sort(ip_pool.begin(), ip_pool.end(), sortCompor);
}

[[nodiscard]] Pool filter(const Pool &pool, std::uint8_t value) {

  Pool ip;

  auto findCompor = [value, &ip](const FormatString &a) {
    auto &&[ip_tuple_a, text_1_a, text_2_a] = a;
    auto &&[ip_0_a, ip_1_a, ip_2_a, ip_3_a] = ip_tuple_a;

    if (ip_0_a == value) {
      ip.push_back(a);
    }
  };

  std::for_each(pool.cbegin(), pool.cend(), findCompor);

  return ip;
}

[[nodiscard]] Pool filter(const Pool &pool, std::uint8_t valueFerst,
                          std::uint8_t valueSecond) {

  std::vector<FormatString> ip;

  auto findCompor = [valueFerst, valueSecond, &ip](const FormatString &a) {
    auto &&[ip_tuple_a, text_1_a, text_2_a] = a;
    auto &&[ip_0_a, ip_1_a, ip_2_a, ip_3_a] = ip_tuple_a;

    if (ip_0_a == valueFerst && ip_1_a == valueSecond) {
      ip.push_back(a);
    }
  };

  std::for_each(pool.cbegin(), pool.cend(), findCompor);

  return ip;
}

[[nodiscard]] Pool filter_any(const Pool &pool, std::uint8_t valueFerst) {

  Pool ip;

  auto findCompor = [valueFerst, &ip](const FormatString &a) {
    auto &&[ip_tuple_a, text_1_a, text_2_a] = a;
    auto &&[ip_0_a, ip_1_a, ip_2_a, ip_3_a] = ip_tuple_a;

    if (ip_0_a == valueFerst || ip_1_a == valueFerst || ip_2_a == valueFerst ||
        ip_3_a == valueFerst) {
      ip.push_back(a);
    }
  };

  std::for_each(pool.cbegin(), pool.cend(), findCompor);

  return ip;
}
