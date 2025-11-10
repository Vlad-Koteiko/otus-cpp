#ifndef BD_HPP
#define BD_HPP

#include <sqlite3.h>

#include <iostream>
#include <map>

namespace bd {

  enum class TABLE { A, B };

  struct Data {
    int id;
    std::string name;
  };

  struct DataBase {
    DataBase() {
      auto open_result = sqlite3_open("db", &db_ptr);
      if (open_result) {
        std::cout << "Unable to open DB" << std::endl;
      }
      createTable(TABLE::A);
      createTable(TABLE::B);
    }

    [[nodiscard]] bool insert(TABLE, const Data&);
    [[nodiscard]] std::map<int,std::string> getTable(TABLE) const;
    [[nodiscard]] bool clearTable(TABLE);

    ~DataBase() {
      if (db_ptr) {
        auto close_result = sqlite3_close(db_ptr);
        if (close_result) {
          std::cout << "Unable to close DB" << std::endl;
        }
      }
    }

   private:
    void createTable(TABLE);
    sqlite3* db_ptr = nullptr;
    [[nodiscard]] std::string tableName(TABLE t) const {
      return (t == TABLE::A) ? "A" : "B";
    }
  };

}  // namespace bd

#endif
