module;

#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <unordered_map>

export module commander;
export import bd;

export struct Boss {
  using Pool = std::vector<std::string>;

  Boss() { initializeHandlers(); }

  [[nodiscard]] std::optional<Pool> run(const std::string&);

 private:
  using CommandHandler = std::function<Pool(std::istringstream&)>;

  enum class Command { INSERT, FIND_NAME, FIND_PHONE, SHOW_ALL, UNKNOWN };

  struct Code {
    static constexpr std::string OK    = "> OK";
    static constexpr std::string ERROR = "> ERR";
  };

  std::unordered_map<Command, CommandHandler> commandHandlers;
  DataBase db;

  [[nodiscard]] Command parseType(const std::string& s) const;
  [[nodiscard]] Pool handleInsert(std::istringstream& iss);
  [[nodiscard]] Pool handleFindName(std::istringstream& iss);
  [[nodiscard]] Pool handleFindPhone(std::istringstream& iss);
  [[nodiscard]] Pool handleShowAll(std::istringstream& iss);
  [[nodiscard]] Pool handleUnknown(std::istringstream& iss);

  void initializeHandlers();
};

std::optional<Boss::Pool> Boss::run(const std::string& line) {
  std::istringstream iss(line);
  std::string cmdWord;
  iss >> cmdWord;

  auto it = commandHandlers.find(parseType(cmdWord));
  if (it != commandHandlers.end()) {
    return it->second(iss);
  }

  return std::nullopt;
}

Boss::Command Boss::parseType(const std::string& s) const {
  if (s == "INSERT")
    return Command::INSERT;
  if (s == "FIND_NAME")
    return Command::FIND_NAME;
  if (s == "FIND_PHONE")
    return Command::FIND_PHONE;
  if (s == "SHOW_ALL")
    return Command::SHOW_ALL;
  return Command::UNKNOWN;
}

void Boss::initializeHandlers() {
  commandHandlers = {
      {Command::INSERT, [this](auto& iss) { return handleInsert(iss); }},
      {Command::FIND_NAME, [this](auto& iss) { return handleFindName(iss); }},
      {Command::FIND_PHONE, [this](auto& iss) { return handleFindPhone(iss); }},
      {Command::SHOW_ALL, [this](auto& iss) { return handleShowAll(iss); }},
      {Command::UNKNOWN, [this](auto& iss) { return handleUnknown(iss); }}};
}

Boss::Pool Boss::handleInsert(std::istringstream& iss) {
  Pool poll;

  Contact contact;
  Phone phone;
  bool statusFind = true;

  iss >> contact.name;
  iss >> phone.phone;
  iss >> phone.type;

  auto results = db.findContactByName(contact.name);

  for (auto& [id, name] : results) {
    if (contact.name == name)
      statusFind = false;
  }

  if (statusFind) {
    auto insert = db.insertContact(std::move(contact));
    auto status = db.insertPhone(insert.value(), phone);
    poll.push_back(Code::OK);
  } else {
    std::string str = Code::ERROR + " duplicate " + contact.name;
    poll.push_back(str);
  }

  return poll;
}

Boss::Pool Boss::handleFindName(std::istringstream& iss) {
  Pool pool;

  Contact contact;

  iss >> contact.name;

  auto result = db.findContactByName(contact.name);

  if (result.size() != 0) {
    for (auto [id, name] : result) {
      auto phone      = db.getPhones(id);
      auto it         = phone.begin();
      std::string str = contact.name + " -> " + it->second;
      pool.push_back(str);
      pool.push_back(Code::OK);
    }
  } else {
    std::string str = Code::ERROR + " NOT FIND " + contact.name;
    pool.push_back(str);
  }
  return pool;
}

Boss::Pool Boss::handleFindPhone([[maybe_unused]] std::istringstream& iss) {
  Pool pool;

  Contact contact;

  iss >> contact.name;

  auto phone = db.findContactByPhone(contact.name);

  if (phone.size() != 0) {
    for (auto [id, name] : phone) {
      std::string str = name + " -> " + contact.name;
      pool.push_back(str);
      pool.push_back(Code::OK);
    }
  } else {
    std::string str = Code::ERROR + " NOT FIND " + contact.name;
    pool.push_back(str);
  }

  return pool;
}

Boss::Pool Boss::handleShowAll([[maybe_unused]] std::istringstream& iss) {
  Pool pool;

  auto contacts = db.getContacts();

  if (contacts.size() != 0) {
    for (auto [id, name] : contacts) {
      auto phone = db.getPhones(id);

      for (auto [id, p] : phone) {
        std::string str = name + " -> " + p;
        pool.push_back(str);
      }
    }
    pool.push_back(Code::OK);
  } else {
    std::string str = Code::ERROR + " NOT FIND ";
    pool.push_back(str);
  }

  return pool;
}

Boss::Pool Boss::handleUnknown([[maybe_unused]] std::istringstream& iss) {
  Pool result;
  result.push_back(Code::ERROR);
  return result;
}
