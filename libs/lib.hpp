#include <boost/crc.hpp>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

namespace cmp = boost::program_options;
namespace fs = boost::filesystem;

using Config = std::unordered_map<std::string, std::string>;

const std::vector options = {"help",    "scan", "exc", "level",
                             "minSize", "blok", "mask"};

int version();

namespace detail {

[[nodiscard]] std::vector<fs::path> getFileName(const Config &config);
[[nodiscard]] bool crc32(const char *fileOne, const char *fileTwo, size_t size);
[[nodiscard]] std::uint32_t getSizeBuffer(std::size_t size, std::size_t blok);
[[nodiscard]] bool compare(const fs::path &file_1, const fs::path &file_2,
                           std::size_t s);
} // namespace detail

[[nodiscard]] Config cmpRead(int argc, char *argv[]);
[[nodiscard]] auto run(const Config &config) -> std::vector<fs::path>;
