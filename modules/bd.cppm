module;

#include <sqlite3.h>
#include <iostream>
#include <map>

export module bd;

export enum class TABLE { A, B };

export struct Data {
  int id;
  std::string name;
};

export struct DataBase {
  DataBase() {
    auto open_result = sqlite3_open("db", &db_ptr);
    if (open_result) {
      std::cout << "Unable to open DB" << std::endl;
    }
    createTable(TABLE::A);
    createTable(TABLE::B);
  }

  [[nodiscard]] bool insert(TABLE, const Data&);
  [[nodiscard]] std::map<int, std::string> getTable(TABLE) const;
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

void DataBase::createTable(TABLE table) {
  const std::string sql = "CREATE TABLE " + tableName(table)
                          + " (id INTEGER PRIMARY KEY, name TEXT)";
  char* errorMessage = nullptr;
  auto rc = sqlite3_exec(db_ptr, sql.c_str(), nullptr, nullptr, &errorMessage);
  if (rc != SQLITE_OK) {
    std::cout << "Unable to tabe " << tableName(table) << std::endl;
  }
}

bool DataBase::insert(TABLE table, const Data& data) {
  char* errorMessage = nullptr;
  bool flag          = true;
  std::string comand = "INSERT INTO " + tableName(table);
  comand += " VALUES('" + std::to_string(data.id) + "','" + data.name + "')";

  int insert = sqlite3_exec(db_ptr, comand.c_str(), 0, 0, &errorMessage);
  if (insert != SQLITE_OK) [[unlikely]] {
    flag = false;
  }
  return flag;
}

std::map<int, std::string> DataBase::getTable(TABLE table) const {
  std::map<int, std::string> result;
  char* errorMessage  = nullptr;
  std::string command = "SELECT * FROM " + tableName(table);

  auto callback = [](void* data,
                     int argc,
                     char** argv,
                     [[maybe_unused]] char** colNames) -> int {
    auto* resultVector = static_cast<std::map<int, std::string>*>(data);

    if (argc >= 2) {
      Data record;
      record.id   = std::stoi(argv[0]);
      record.name = argv[1];
      resultVector->insert({record.id, record.name});
    }

    return 0;
  };

  int select =
      sqlite3_exec(db_ptr, command.c_str(), callback, &result, &errorMessage);

  if (select != SQLITE_OK) [[unlikely]] {
    std::cout << "SQL error: " << errorMessage << std::endl;
    sqlite3_free(errorMessage);
    return std::map<int, std::string>();
  }

  return result;
}

bool DataBase::clearTable(TABLE table) {
  char* errorMessage        = nullptr;
  bool flag                 = true;
  const std::string command = "DELETE FROM " + tableName(table);

  int deleteResult = sqlite3_exec(db_ptr, command.c_str(), 0, 0, &errorMessage);

  if (deleteResult != SQLITE_OK) [[unlikely]] {
    std::cerr << "SQL error in clearTable: " << errorMessage << std::endl;
    flag = false;
    sqlite3_free(errorMessage);
  }

  return flag;
}
