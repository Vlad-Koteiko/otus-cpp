#define BOOST_TEST_MODULE test_version

#include <boost/test/unit_test.hpp>

#include "libs/bd.hpp"
#include "libs/commander.hpp"

BOOST_AUTO_TEST_SUITE(test_version)

BOOST_AUTO_TEST_CASE(TestBD) {
  bd::DataBase db;

  auto f = db.insert(bd::TABLE::A, {4, "test1"});
  f      = db.insert(bd::TABLE::A, {5, "test2"});
  if (f) {
  }
  auto t = db.getTable(bd::TABLE::A);

  for (auto tt : t) {
    std::cout << "A id ->" << tt.first << " name ->" << tt.second << std::endl;
  }
  f = db.clearTable(bd::TABLE::A);
  std::cout << "claer teble --------------------------" << std::endl;
  t = db.getTable(bd::TABLE::A);

  for (auto tt : t) {
    std::cout << "id ->" << tt.first << " name ->" << tt.second << std::endl;
  }

  f = db.insert(bd::TABLE::B, {4, "test3"});
  f = db.insert(bd::TABLE::B, {5, "test4"});
  if (f) {
  }
  t = db.getTable(bd::TABLE::B);

  for (auto tt : t) {
    std::cout << "B id ->" << tt.first << " name ->" << tt.second << std::endl;
  }
  f = db.clearTable(bd::TABLE::B);
  std::cout << "claer teble --------------------------" << std::endl;
  t = db.getTable(bd::TABLE::B);

  for (auto tt : t) {
    std::cout << "id ->" << tt.first << " name ->" << tt.second << std::endl;
  }
}

BOOST_AUTO_TEST_CASE(TestCommander) {
  std::cout << "------------COMMANDER----------------" << std::endl;
  commander::Boss boss;

  auto f = boss.run("INSERT A 0 lean");

  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }

  f = boss.run("INSERT A 0 understand");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("INSERT A 1 sweater");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("INSERT A 11 sweater");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("INSERT A 2 frank");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("INSERT B 6 flour");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("INSERT B 7 wonder");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("INSERT B 0 selection");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("INSERT B 1 flour");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("INSERT B 2 wonder");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("INSERT B 8 selection");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  // f = boss.run("TRUNCATE A");
  // std::cout << f << std::endl;

  f = boss.run("INTERSECTION");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  f = boss.run("SYMMETRIC_DIFFERENCE");
  for (auto s : f.value()) {
    std::cout << s << std::endl;
  }
  // f = boss.run("SYMMETRIC_");
  // for (auto s : f.value()) {
  //   std::cout << s << std::endl;
  // }
}

BOOST_AUTO_TEST_SUITE_END()
