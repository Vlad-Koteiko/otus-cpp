module;

#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <unordered_map>

export module commander;
export import bd;

// export struct Boss {
//   using Pool = std::vector<std::string>;
//
//   Boss() { initializeHandlers(); }
//
//   [[nodiscard]] std::optional<Pool> run(const std::string&);
//
//  private:
//   using CommandHandler = std::function<Pool(std::istringstream&)>;
//
//   enum class Command {
//     INSERT,
//     TRUNCATE,
//     INTERSECTION,
//     SYMMETRIC_DIFFERENCE,
//     UNKNOWN
//   };
//
//   struct Code {
//     static constexpr std::string OK    = "> OK";
//     static constexpr std::string ERROR = "> ERR";
//   };
//
//   std::unordered_map<Command, CommandHandler> commandHandlers;
//   DataBase db;
//
//   [[nodiscard]] Command parseType(const std::string& s) const;
//   [[nodiscard]] Pool handleInsert(std::istringstream& iss);
//   [[nodiscard]] Pool handleTruncate(std::istringstream& iss);
//   [[nodiscard]] Pool handleIntersection(std::istringstream& iss);
//   [[nodiscard]] Pool handleSymmetric(std::istringstream& iss);
//   [[nodiscard]] Pool handleUnknown(std::istringstream& iss);
//
//   void initializeHandlers();
//
//   [[nodiscard]] TABLE tableName(const std::string& t) const {
//     return (t == "A") ? TABLE::A : TABLE::B;
//   }
// };
//
// std::optional<Boss::Pool> Boss::run(const std::string& line) {
//   std::istringstream iss(line);
//   std::string cmdWord;
//   iss >> cmdWord;
//
//   auto it = commandHandlers.find(parseType(cmdWord));
//   if (it != commandHandlers.end()) {
//     return it->second(iss);
//   }
//
//   return std::nullopt;
// }
//
// Boss::Command Boss::parseType(const std::string& s) const {
//   if (s == "INSERT")
//     return Command::INSERT;
//   if (s == "TRUNCATE")
//     return Command::TRUNCATE;
//   if (s == "INTERSECTION")
//     return Command::INTERSECTION;
//   if (s == "SYMMETRIC_DIFFERENCE")
//     return Command::SYMMETRIC_DIFFERENCE;
//   return Command::UNKNOWN;
// }
//
// void Boss::initializeHandlers() {
//   commandHandlers = {
//       {Command::INSERT, [this](auto& iss) { return handleInsert(iss); }},
//       {Command::TRUNCATE, [this](auto& iss) { return handleTruncate(iss); }},
//       {Command::INTERSECTION,
//        [this](auto& iss) { return handleIntersection(iss); }},
//       {Command::SYMMETRIC_DIFFERENCE,
//        [this](auto& iss) { return handleSymmetric(iss); }},
//       {Command::UNKNOWN, [this](auto& iss) { return handleUnknown(iss); }}};
// }
//
// Boss::Pool Boss::handleInsert(std::istringstream& iss) {
//   Pool poll;
//   std::string table;
//   Data data;
//   iss >> table;
//   iss >> data.id;
//   iss >> data.name;
//
//   auto insert = db.insert(tableName(table), std::move(data));
//
//   if (insert) {
//     poll.push_back(Code::OK);
//   } else {
//     std::string str = Code::ERROR + " duplicate " + std::to_string(data.id);
//     poll.push_back(str);
//   }
//   return poll;
// }
//
// Boss::Pool Boss::handleTruncate(std::istringstream& iss) {
//   Pool pool;
//   std::string table;
//   iss >> table;
//
//   auto insert = db.clearTable(tableName(table));
//
//   if (insert) {
//     pool.push_back(Code::OK);
//   }
//   pool.push_back(Code::ERROR + " truncate " + table);
//   return pool;
// }
//
// Boss::Pool Boss::handleIntersection([[maybe_unused]] std::istringstream& iss) {
//   Pool result;
//
//   auto tableA = db.getTable(TABLE::A);
//   auto tableB = db.getTable(TABLE::B);
//
//   for (const auto& [id, data] : tableA) {
//     auto it = tableB.find(id);
//     if (it != tableB.end()) {
//       const std::string str =
//           "< " + std::to_string(id) + "," + data + "," + it->second;
//
//       result.push_back(str);
//     }
//   }
//   if (!result.empty()) {
//     result.push_back(Code::OK);
//   } else {
//     result.push_back(Code::ERROR);
//   }
//
//   return result;
// }
//
// Boss::Pool Boss::handleSymmetric([[maybe_unused]] std::istringstream& iss) {
//   Pool result;
//
//   auto tableA = db.getTable(TABLE::A);
//   auto tableB = db.getTable(TABLE::B);
//
//   for (const auto& [id, data] : tableA) {
//     std::string str;
//     if (!tableB.count(id)) {
//       str += "< " + std::to_string(id) + "," + data + ",";
//       result.push_back(str);
//     }
//   }
//
//   for (const auto& [id, data] : tableB) {
//     std::string str;
//     if (!tableA.count(id)) {
//       str += "< " + std::to_string(id) + ",," + data;
//       result.push_back(str);
//     }
//   }
//
//   if (!result.empty()) {
//     result.push_back(Code::OK);
//   } else {
//     result.push_back(Code::ERROR);
//   }
//
//   return result;
// }
//
// Boss::Pool Boss::handleUnknown([[maybe_unused]] std::istringstream& iss) {
//   Pool result;
//   result.push_back(Code::ERROR);
//   return result;
// }

