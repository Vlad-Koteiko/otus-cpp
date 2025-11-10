
#include <functional>
#include <iostream>
#include <optional>
#include <sstream>
#include <unordered_map>

#include "bd.hpp"

namespace commander {

  struct Boss {
    using Pool = std::vector<std::string>;

    Boss() { initializeHandlers(); }

    [[nodiscard]] std::optional<Pool> run(const std::string&);

   private:
    using CommandHandler = std::function<Pool(std::istringstream&)>;

    enum class Command {
      INSERT,
      TRUNCATE,
      INTERSECTION,
      SYMMETRIC_DIFFERENCE,
      UNKNOWN
    };

    struct Code {
      static constexpr std::string OK    = "> OK";
      static constexpr std::string ERROR = "> ERR";
    };

    std::unordered_map<Command, CommandHandler> commandHandlers;
    bd::DataBase db;

    [[nodiscard]] Command parseType(const std::string& s) const;
    [[nodiscard]] Pool handleInsert(std::istringstream& iss);
    [[nodiscard]] Pool handleTruncate(std::istringstream& iss);
    [[nodiscard]] Pool handleIntersection(std::istringstream& iss);
    [[nodiscard]] Pool handleSymmetric(std::istringstream& iss);
    [[nodiscard]] Pool handleUnknown(std::istringstream& iss);

    void initializeHandlers();

    [[nodiscard]] bd::TABLE tableName(const std::string& t) const {
      return (t == "A") ? bd::TABLE::A : bd::TABLE::B;
    }
  };
}  // namespace commander
