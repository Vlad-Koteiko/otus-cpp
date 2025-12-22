#define BOOST_TEST_MODULE test_version

#include <boost/test/unit_test.hpp>
#include <filesystem>

import commander;

BOOST_AUTO_TEST_SUITE(test_version)

struct DBFixture {
  DBFixture() {
    std::filesystem::remove("phonebook.db");
    db = std::make_unique<DataBase>();
  }

  ~DBFixture() {
    db.reset();
    std::filesystem::remove("phonebook.db");
  }

  std::unique_ptr<DataBase> db;
};

BOOST_FIXTURE_TEST_CASE(insert_contact, DBFixture) {
  Contact c {1, "Иван Иванов"};

  BOOST_CHECK(db->insertContact(c));

  auto contacts = db->getContacts();
  BOOST_CHECK_EQUAL(contacts.size(), 1);
  BOOST_CHECK_EQUAL(contacts[1], "Иван Иванов");
}

BOOST_FIXTURE_TEST_CASE(insert_phone, DBFixture) {
  db->insertContact({1, "Иван Иванов"});

  Phone p1 {1, 1, "+79991234567", "mobile"};
  Phone p2 {2, 1, "+74951234567", "work"};

  BOOST_CHECK(db->insertPhone(p1));
  BOOST_CHECK(db->insertPhone(p2));

  auto phones = db->getPhones(1);
  BOOST_CHECK_EQUAL(phones.size(), 2);
}

BOOST_FIXTURE_TEST_CASE(contact_phone_relation, DBFixture) {
  db->insertContact({1, "Иван Иванов"});
  db->insertContact({2, "Петр Петров"});

  db->insertPhone({1, 1, "+79991234567", "mobile"});
  db->insertPhone({2, 2, "+70001112233", "home"});

  auto phones1 = db->getPhones(1);
  auto phones2 = db->getPhones(2);

  BOOST_CHECK_EQUAL(phones1.size(), 1);
  BOOST_CHECK_EQUAL(phones2.size(), 1);

  BOOST_CHECK_EQUAL(phones1.begin()->second, "+79991234567");
  BOOST_CHECK_EQUAL(phones2.begin()->second, "+70001112233");
}

BOOST_FIXTURE_TEST_CASE(empty_phones, DBFixture) {
  db->insertContact({1, "Иван Иванов"});

  auto phones = db->getPhones(1);
  BOOST_CHECK(phones.empty());
}

BOOST_FIXTURE_TEST_CASE(duplicate_contact_id, DBFixture) {
  BOOST_CHECK(db->insertContact({1, "Иван"}));
  BOOST_CHECK(!db->insertContact({1, "Петр"}));  // PRIMARY KEY
}

BOOST_AUTO_TEST_SUITE_END()
