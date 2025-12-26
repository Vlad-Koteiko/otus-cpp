#define BOOST_TEST_MODULE test_version

#include <boost/test/unit_test.hpp>
#include <filesystem>
#include <iostream>

import bd;
import commander;

BOOST_AUTO_TEST_SUITE(test_version)

struct DBFixture {
  DBFixture() {
    std::filesystem::remove("phonebook.db");
    db   = std::make_unique<DataBase>();
    boss = std::make_unique<Boss>();
  }

  ~DBFixture() {
    db.reset();
    std::filesystem::remove("phonebook.db");
  }

  std::unique_ptr<DataBase> db;
  std::unique_ptr<Boss> boss;
};

BOOST_FIXTURE_TEST_CASE(insert_contact, DBFixture) {
  Contact c {"Иван Иванов"};

  auto id = db->insertContact(c);
  BOOST_REQUIRE(id.has_value());

  auto contacts = db->getContacts();
  BOOST_CHECK_EQUAL(contacts.size(), 1);
  BOOST_CHECK_EQUAL(contacts[id.value()], "Иван Иванов");
}

BOOST_FIXTURE_TEST_CASE(insert_phone, DBFixture) {
  auto contact_id = db->insertContact({"Иван Иванов"});
  BOOST_REQUIRE(contact_id.has_value());

  Phone p1 {"+37591234567", "mobile"};
  Phone p2 {"+37551234567", "work"};

  BOOST_CHECK(db->insertPhone(contact_id.value(), p1));
  BOOST_CHECK(db->insertPhone(contact_id.value(), p2));

  auto phones = db->getPhones(contact_id.value());
  BOOST_CHECK_EQUAL(phones.size(), 2);
}

BOOST_FIXTURE_TEST_CASE(contact_phone_relation, DBFixture) {
  auto id1 = db->insertContact({"Иван Иванов"});
  auto id2 = db->insertContact({"Петр Петров"});
  BOOST_REQUIRE(id1.has_value());
  BOOST_REQUIRE(id2.has_value());

  auto a = db->insertPhone(id1.value(), {"+37591234567", "mobile"});
  a      = db->insertPhone(id2.value(), {"+37501112233", "home"});

  auto phones1 = db->getPhones(id1.value());
  auto phones2 = db->getPhones(id2.value());

  BOOST_CHECK_EQUAL(phones1.size(), 1);
  BOOST_CHECK_EQUAL(phones2.size(), 1);

  BOOST_CHECK_EQUAL(phones1.begin()->second, "+37591234567");
  BOOST_CHECK_EQUAL(phones2.begin()->second, "+37501112233");
}

BOOST_FIXTURE_TEST_CASE(empty_phones, DBFixture) {
  auto id = db->insertContact({"Иван Иванов"});
  BOOST_REQUIRE(id.has_value());

  auto phones = db->getPhones(id.value());
  BOOST_CHECK(phones.empty());
}

BOOST_FIXTURE_TEST_CASE(search_by_name, DBFixture) {
  auto id1 = db->insertContact({"Иван Иванов"});
  auto id2 = db->insertContact({"Иван Петров"});
  auto id3 = db->insertContact({"Петр Петров"});
  BOOST_REQUIRE(id1.has_value());
  BOOST_REQUIRE(id2.has_value());
  BOOST_REQUIRE(id3.has_value());

  auto results = db->findContactByName("Иван");

  for (auto& [id, name] : results) {
    // std::cout << id << ": " << name << "\n";
  }

  BOOST_CHECK_EQUAL(results.size(), 2);
  BOOST_CHECK(results.count(id1.value()) > 0);
  BOOST_CHECK(results.count(id2.value()) > 0);
}

BOOST_FIXTURE_TEST_CASE(search_by_phone, DBFixture) {
  auto id1 = db->insertContact({"Иван Иванов"});
  auto id2 = db->insertContact({"Петр Петров"});
  BOOST_REQUIRE(id1.has_value());
  BOOST_REQUIRE(id2.has_value());

  auto a = db->insertPhone(id1.value(), {"+37591234567", "mobile"});
  a      = db->insertPhone(id2.value(), {"+37501112233", "home"});

  auto results = db->findContactByPhone("+37501112233");
  BOOST_CHECK_EQUAL(results.size(), 1);
  BOOST_CHECK(results.count(id2.value()) > 0);
}

BOOST_FIXTURE_TEST_CASE(insert, DBFixture) {
  const std::string str = "INSERT INAV +37529111111 work";

  auto p = boss->run(str);

  for (const auto& str : p.value()) {
    BOOST_CHECK_EQUAL(str, "> OK");
  }

  p = boss->run(str);

  for (const auto& str : p.value()) {
    BOOST_CHECK_EQUAL(str, "> ERR duplicate INAV");
  }
}

BOOST_FIXTURE_TEST_CASE(findName, DBFixture) {
  std::string str = "INSERT INAV +37529111111 work";
  auto p          = boss->run(str);

  str = "INSERT PETROV +37529111111 work";
  p   = boss->run(str);

  str = "FIND_NAME INAV";

  p = boss->run(str);

  BOOST_CHECK_EQUAL(p.value()[0], "INAV -> +37529111111");

  str = "FIND_NAME INAVV";

  p = boss->run(str);

  BOOST_CHECK_EQUAL(p.value()[0], "> ERR NOT FIND INAVV");
}

BOOST_FIXTURE_TEST_CASE(findPhone, DBFixture) {
  std::string str = "INSERT INAV +37529111111 work";
  auto p          = boss->run(str);

  str = "INSERT PETROV +37529111211 work";
  p   = boss->run(str);

  str = "FIND_PHONE +37529111211";

  p = boss->run(str);

  BOOST_CHECK_EQUAL(p.value()[0], "PETROV -> +37529111211");

  str = "FIND_PHONE +37529111221";

  p = boss->run(str);

  BOOST_CHECK_EQUAL(p.value()[0], "> ERR NOT FIND +37529111221");
}

BOOST_FIXTURE_TEST_CASE(showAll, DBFixture) {
  std::string str = "INSERT INAV +37529111111 work";
  auto p          = boss->run(str);

  str = "INSERT PETROV +37529111211 work";
  p   = boss->run(str);

  str = "INSERT POPOV +37529111211 work";
  p   = boss->run(str);

  str = "SHOW_ALL";

  p = boss->run(str);

  BOOST_CHECK_EQUAL(p.value()[0], "INAV -> +37529111111");
  BOOST_CHECK_EQUAL(p.value()[1], "PETROV -> +37529111211");
  BOOST_CHECK_EQUAL(p.value()[2], "POPOV -> +37529111211");

  for (const auto& str : p.value()) {
    // std::cout << str << std::endl;
  }
}
BOOST_AUTO_TEST_SUITE_END()
