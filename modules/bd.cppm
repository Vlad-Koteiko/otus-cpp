module;

#include <sqlite3.h>

#include <iostream>
#include <map>

export module bd;

export enum class TABLE { CONTACTS, PHONES };

// export enum class TABLE { A, B };
//
// export struct Data {
//   int id;
//   std::string name;
// };

export struct Contact {
  int id;
  std::string name;
};

export struct Phone {
  int id;
  int contact_id;
  std::string phone;
  std::string type;  // mobile / home / work
};

export struct DataBase {
  DataBase() {
    if (sqlite3_open("phonebook.db", &db_ptr) != SQLITE_OK) {
      std::cerr << "Unable to open DB\n";
    }

    createTable(TABLE::CONTACTS);
    createTable(TABLE::PHONES);
  }

  [[nodiscard]] bool insertContact(const Contact&);
  [[nodiscard]] bool insertPhone(const Phone&);

  [[nodiscard]] std::map<int, std::string> getContacts() const;
  [[nodiscard]] std::map<int, std::string> getPhones(int contact_id) const;

  ~DataBase() {
    if (db_ptr)
      sqlite3_close(db_ptr);
  }

 private:
  sqlite3* db_ptr = nullptr;

  [[nodiscard]] std::string tableName(TABLE t) const {
    return (t == TABLE::CONTACTS) ? "contacts" : "phones";
  }

  void createTable(TABLE);
};

// void DataBase::createTable(TABLE table) {
//   const std::string sql = "CREATE TABLE " + tableName(table)
//                           + " (id INTEGER PRIMARY KEY, name TEXT)";
//   char* errorMessage = nullptr;
//   auto rc = sqlite3_exec(db_ptr, sql.c_str(), nullptr, nullptr,
//   &errorMessage); if (rc != SQLITE_OK) {
//     std::cout << "Unable to tabe " << tableName(table) << std::endl;
//   }
// }
//

void DataBase::createTable(TABLE table) {
  std::string sql;

  if (table == TABLE::CONTACTS) {
    sql =
        "CREATE TABLE IF NOT EXISTS contacts ("
        "id INTEGER PRIMARY KEY, "
        "name TEXT NOT NULL);";
  } else {
    sql =
        "CREATE TABLE IF NOT EXISTS phones ("
        "id INTEGER PRIMARY KEY, "
        "contact_id INTEGER NOT NULL, "
        "phone TEXT NOT NULL, "
        "type TEXT, "
        "FOREIGN KEY(contact_id) REFERENCES contacts(id));";
  }

  char* err = nullptr;
  if (sqlite3_exec(db_ptr, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
    std::cerr << "Create table error: " << err << "\n";
    sqlite3_free(err);
  }
}

// bool DataBase::insert(TABLE table, const Data& data) {
//   char* errorMessage = nullptr;
//   bool flag          = true;
//   std::string comand = "INSERT INTO " + tableName(table);
//   comand += " VALUES('" + std::to_string(data.id) + "','" + data.name + "')";
//
//   int insert = sqlite3_exec(db_ptr, comand.c_str(), 0, 0, &errorMessage);
//   if (insert != SQLITE_OK) [[unlikely]] {
//     flag = false;
//   }
//   return flag;
// }

bool DataBase::insertContact(const Contact& c) {
  std::string sql = "INSERT INTO contacts VALUES(" + std::to_string(c.id)
                    + ", '" + c.name + "');";

  char* err = nullptr;
  if (sqlite3_exec(db_ptr, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
    sqlite3_free(err);
    return false;
  }
  return true;
}

bool DataBase::insertPhone(const Phone& p) {
  std::string sql = "INSERT INTO phones VALUES(" + std::to_string(p.id) + ", "
                    + std::to_string(p.contact_id) + ", '" + p.phone + "', '"
                    + p.type + "');";

  char* err = nullptr;
  if (sqlite3_exec(db_ptr, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
    sqlite3_free(err);
    return false;
  }
  return true;
}

//
// std::map<int, std::string> DataBase::getTable(TABLE table) const {
//   std::map<int, std::string> result;
//   char* errorMessage  = nullptr;
//   std::string command = "SELECT * FROM " + tableName(table);
//
//   auto callback = [](void* data,
//                      int argc,
//                      char** argv,
//                      [[maybe_unused]] char** colNames) -> int {
//     auto* resultVector = static_cast<std::map<int, std::string>*>(data);
//
//     if (argc >= 2) {
//       Data record;
//       record.id   = std::stoi(argv[0]);
//       record.name = argv[1];
//       resultVector->insert({record.id, record.name});
//     }
//
//     return 0;
//   };
//
//   int select =
//       sqlite3_exec(db_ptr, command.c_str(), callback, &result,
//       &errorMessage);
//
//   if (select != SQLITE_OK) [[unlikely]] {
//     std::cout << "SQL error: " << errorMessage << std::endl;
//     sqlite3_free(errorMessage);
//     return std::map<int, std::string>();
//   }
//
//   return result;
// }

std::map<int, std::string> DataBase::getContacts() const {
  std::map<int, std::string> result;

  auto callback = [](void* data, int argc, char** argv, char**) {
    auto* map = static_cast<std::map<int, std::string>*>(data);
    map->insert({std::stoi(argv[0]), argv[1]});
    return 0;
  };

  char* err = nullptr;
  sqlite3_exec(db_ptr, "SELECT * FROM contacts;", callback, &result, &err);
  return result;
}

std::map<int, std::string> DataBase::getPhones(int contact_id) const {
  std::map<int, std::string> result;

  std::string sql = "SELECT id, phone FROM phones WHERE contact_id = "
                    + std::to_string(contact_id) + ";";

  auto callback = [](void* data, int argc, char** argv, char**) {
    auto* map = static_cast<std::map<int, std::string>*>(data);
    map->insert({std::stoi(argv[0]), argv[1]});
    return 0;
  };

  char* err = nullptr;
  sqlite3_exec(db_ptr, sql.c_str(), callback, &result, &err);
  return result;
}

// bool DataBase::clearTable(TABLE table) {
//   char* errorMessage        = nullptr;
//   bool flag                 = true;
//   const std::string command = "DELETE FROM " + tableName(table);
//
//   int deleteResult = sqlite3_exec(db_ptr, command.c_str(), 0, 0,
//   &errorMessage);
//
//   if (deleteResult != SQLITE_OK) [[unlikely]] {
//     std::cerr << "SQL error in clearTable: " << errorMessage << std::endl;
//     flag = false;
//     sqlite3_free(errorMessage);
//   }
//
//   return flag;
// }
