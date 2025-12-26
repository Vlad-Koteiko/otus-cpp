module;

#include <sqlite3.h>

#include <iostream>
#include <map>

export module bd;

export enum class TABLE { CONTACTS, PHONES };

export struct Contact {
  std::string name;
};

export struct Phone {
  std::string phone;
  std::string type;
};

export struct DataBase {
  DataBase();

  [[nodiscard]] std::optional<int> insertContact(const Contact&);
  [[nodiscard]] bool insertPhone(int contact_id, const Phone&);

  [[nodiscard]] std::map<int, std::string> getContacts() const;
  [[nodiscard]] std::map<int, std::string> getPhones(int contact_id) const;

  [[nodiscard]] std::map<int, std::string> findContactByPhone(
      const std::string& phone) const;
  [[nodiscard]] std::map<int, std::string> findContactByName(
      const std::string& namePart) const;

 private:
  sqlite3* db_ptr = nullptr;
  void createTable(TABLE);
};

DataBase::DataBase() {
  if (sqlite3_open("phonebook.db", &db_ptr) != SQLITE_OK) {
    std::cerr << "Unable to open DB\n";
  }

  // ВАЖНО для связей
  sqlite3_exec(db_ptr, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);

  createTable(TABLE::CONTACTS);
  createTable(TABLE::PHONES);
}

void DataBase::createTable(TABLE table) {
  std::string sql;

  if (table == TABLE::CONTACTS) {
    sql =
        "CREATE TABLE IF NOT EXISTS contacts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "name TEXT NOT NULL);";
  } else {
    sql =
        "CREATE TABLE IF NOT EXISTS phones ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "contact_id INTEGER NOT NULL, "
        "phone TEXT NOT NULL, "
        "type TEXT, "
        "FOREIGN KEY(contact_id) REFERENCES contacts(id) ON DELETE CASCADE);";
  }

  char* err = nullptr;
  if (sqlite3_exec(db_ptr, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
    std::cerr << "Create table error: " << err << "\n";
    sqlite3_free(err);
  }
}

std::optional<int> DataBase::insertContact(const Contact& c) {
  std::string sql = "INSERT INTO contacts(name) VALUES('" + c.name + "');";

  char* err = nullptr;
  if (sqlite3_exec(db_ptr, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
    sqlite3_free(err);
    return std::nullopt;
  }

  return static_cast<int>(sqlite3_last_insert_rowid(db_ptr));
}

bool DataBase::insertPhone(int contact_id, const Phone& p) {
  std::string sql = "INSERT INTO phones(contact_id, phone, type) VALUES("
                    + std::to_string(contact_id) + ", '" + p.phone + "', '"
                    + p.type + "');";

  char* err = nullptr;
  if (sqlite3_exec(db_ptr, sql.c_str(), nullptr, nullptr, &err) != SQLITE_OK) {
    sqlite3_free(err);
    return false;
  }
  return true;
}

std::map<int, std::string> DataBase::getContacts() const {
  std::map<int, std::string> result;

  auto callback = [](void* data, int argc, char** argv, char**) {
    auto* map = static_cast<std::map<int, std::string>*>(data);
    map->insert({std::stoi(argv[0]), argv[1]});
    return 0;
  };

  char* err = nullptr;
  sqlite3_exec(db_ptr,
               "SELECT id, name FROM contacts;",
               callback,
               &result,
               &err);
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

std::map<int, std::string> DataBase::findContactByPhone(
    const std::string& phone) const {
  std::map<int, std::string> result;

  std::string sql =
      "SELECT c.id, c.name "
      "FROM contacts c "
      "JOIN phones p ON p.contact_id = c.id "
      "WHERE p.phone = '"
      + phone + "';";

  auto callback = [](void* data, int argc, char** argv, char**) {
    auto* map = static_cast<std::map<int, std::string>*>(data);
    map->insert({std::stoi(argv[0]), argv[1]});
    return 0;
  };

  char* err = nullptr;
  sqlite3_exec(db_ptr, sql.c_str(), callback, &result, &err);

  return result;
}

std::map<int, std::string> DataBase::findContactByName(
    const std::string& namePart) const {
  std::map<int, std::string> result;

  std::string sql =
      "SELECT id, name "
      "FROM contacts "
      "WHERE name LIKE '%"
      + namePart + "%';";

  auto callback = [](void* data, int argc, char** argv, char**) {
    auto* map = static_cast<std::map<int, std::string>*>(data);
    map->insert({std::stoi(argv[0]), argv[1]});
    return 0;
  };

  char* err = nullptr;
  sqlite3_exec(db_ptr, sql.c_str(), callback, &result, &err);

  return result;
}
