#define BOOST_TEST_MODULE test_version

#include "libs/lib.hpp"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(test_valid_version) { BOOST_CHECK(version() > 0); }

BOOST_AUTO_TEST_CASE(TestGetIp) {
  auto ip1 = getIp("192.168.1.1");
  BOOST_CHECK(ip1 == std::make_tuple(192, 168, 1, 1));

  auto ip2 = getIp("0.0.0.0");
  BOOST_CHECK(ip2 == std::make_tuple(0, 0, 0, 0));

  auto ip3 = getIp("255.255.255.255");
  BOOST_CHECK(ip3 == std::make_tuple(255, 255, 255, 255));

  auto ip4 = getIp("1.2.3.4");
  BOOST_CHECK(ip4 == std::make_tuple(1, 2, 3, 4));
}

BOOST_AUTO_TEST_CASE(TestGetFormatString) {
  std::string input = "192.168.1.1\tabc\tdef";
  auto formatStr = getFormatString(input);

  auto &&[ip_tuple, text1, text2] = formatStr;
  auto &&[ip0, ip1, ip2, ip3] = ip_tuple;

  BOOST_CHECK(ip0 == 192);
  BOOST_CHECK(ip1 == 168);
  BOOST_CHECK(ip2 == 1);
  BOOST_CHECK(ip3 == 1);
  BOOST_CHECK(text1 == "abc");
  BOOST_CHECK(text2 == "def");
}

BOOST_AUTO_TEST_CASE(TestSort) {
  Pool pool = {std::make_tuple(std::make_tuple(1, 1, 1, 1), "a", "b"),
               std::make_tuple(std::make_tuple(2, 2, 2, 2), "c", "d"),
               std::make_tuple(std::make_tuple(1, 2, 3, 4), "e", "f"),
               std::make_tuple(std::make_tuple(1, 1, 1, 2), "g", "h")};

  sort(pool);

  // Check the order after sorting
  auto &&[ip1, t1, t2] = pool[0];
  auto &&[ip2, t3, t4] = pool[1];
  auto &&[ip3, t5, t6] = pool[2];
  auto &&[ip4, t7, t8] = pool[3];

  BOOST_CHECK(ip1 == std::make_tuple(2, 2, 2, 2));
  BOOST_CHECK(ip2 == std::make_tuple(1, 2, 3, 4));
  BOOST_CHECK(ip3 == std::make_tuple(1, 1, 1, 2));
  BOOST_CHECK(ip4 == std::make_tuple(1, 1, 1, 1));
}

BOOST_AUTO_TEST_CASE(TestFilterSingleByte) {
  Pool pool = {std::make_tuple(std::make_tuple(1, 1, 1, 1), "a", "b"),
               std::make_tuple(std::make_tuple(2, 2, 2, 2), "c", "d"),
               std::make_tuple(std::make_tuple(1, 2, 3, 4), "e", "f"),
               std::make_tuple(std::make_tuple(1, 1, 1, 2), "g", "h"),
               std::make_tuple(std::make_tuple(3, 1, 1, 1), "i", "j")};

  auto filtered = filter(pool, 1);

  BOOST_CHECK(filtered.size() == 3);
  for (const auto &item : filtered) {
    auto &&[ip, t1, t2] = item;
    auto &&[ip0, ip1, ip2, ip3] = ip;
    BOOST_CHECK(ip0 == 1);
  }
}

BOOST_AUTO_TEST_CASE(TestFilterTwoBytes) {
  Pool pool = {std::make_tuple(std::make_tuple(1, 1, 1, 1), "a", "b"),
               std::make_tuple(std::make_tuple(1, 2, 2, 2), "c", "d"),
               std::make_tuple(std::make_tuple(1, 2, 3, 4), "e", "f"),
               std::make_tuple(std::make_tuple(2, 1, 1, 2), "g", "h"),
               std::make_tuple(std::make_tuple(1, 2, 1, 1), "i", "j")};

  auto filtered = filter(pool, 1, 2);

  BOOST_CHECK(filtered.size() == 3);
  for (const auto &item : filtered) {
    auto &&[ip, t1, t2] = item;
    auto &&[ip0, ip1, ip2, ip3] = ip;
    BOOST_CHECK(ip0 == 1);
    BOOST_CHECK(ip1 == 2);
  }
}

BOOST_AUTO_TEST_CASE(TestFilterAny) {
  Pool pool = {std::make_tuple(std::make_tuple(1, 2, 3, 4), "a", "b"),
               std::make_tuple(std::make_tuple(5, 6, 7, 8), "c", "d"),
               std::make_tuple(std::make_tuple(9, 1, 2, 3), "e", "f"),
               std::make_tuple(std::make_tuple(4, 5, 6, 1), "g", "h"),
               std::make_tuple(std::make_tuple(2, 3, 4, 5), "i", "j")};

  auto filtered = filter_any(pool, 1);

  BOOST_CHECK(filtered.size() == 3);
  for (const auto &item : filtered) {
    auto &&[ip, t1, t2] = item;
    auto &&[ip0, ip1, ip2, ip3] = ip;
    bool hasOne = (ip0 == 1) || (ip1 == 1) || (ip2 == 1) || (ip3 == 1);
    BOOST_CHECK(hasOne);
  }
}

BOOST_AUTO_TEST_SUITE_END()
