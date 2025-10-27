#include "lib.hpp"
#include "version.h"

int version() { return PROJECT_VERSION_PATCH; }

namespace detail {

std::vector<fs::path> getFileName(const Config &config) {

  std::vector<fs::path> pool;
  auto path = config.find("scan")->second;
  auto exc = config.find("exc")->second;
  std::size_t level = std::stoul(config.find("level")->second);
  std::size_t size = std::stoul(config.find("minSize")->second);

  if(level > 1) {
    std::cout << "incorrect argument level" << std::endl;
    exit(0);
  }

  if (level == 1) {

    for (const auto &entry : fs::recursive_directory_iterator(path)) {
      if ((entry != exc) && (entry.path().extension() == ".txt") &&
          (entry.path().size() > size)) {
        pool.push_back(entry.path());
      }
    }

  } else {

    for (const auto &entry : fs::directory_iterator(path)) {
      if ((entry != exc) && (entry.path().extension() == ".txt") &&
          (entry.path().size() > size)) {
        pool.push_back(entry.path());
      }
    }
  }

  return pool;
}

bool crc32(const char *fileOne, const char *fileTwo, size_t size) {

  boost::crc_32_type file_1;
  boost::crc_32_type file_2;

  file_1.process_bytes(fileOne, size);
  file_2.process_bytes(fileTwo, size);

  return file_1.checksum() == file_2.checksum();
}

[[nodiscard]] std::uint32_t getSizeBuffer(std::size_t size, std::size_t blok) {

  if ((size % blok) == 0) {
    return size;
  } else {
    return ((size / blok) * blok) + blok;
  }
}

[[nodiscard]] bool compare(const fs::path &file_1, const fs::path &file_2,
                           std::size_t s) {

  std::ifstream sourse(file_1, std::ios::binary);
  std::ifstream com(file_2, std::ios::binary);

  std::string str(getSizeBuffer(fs::file_size(file_1), s), '\0');
  std::string comStr(getSizeBuffer(fs::file_size(file_2), s), '\0');

  sourse.read(str.data(), '\n');
  com.read(comStr.data(), '\n');

  if (str.size() != comStr.size())
    return false;

  for (size_t i = 0; i < str.size(); i += s) {
    if (!crc32(str.data() + i, comStr.data() + i, s)) {
      return false;
    }
  }

  return true;
}

} // namespace detail

Config cmpRead(int argc, char *argv[]) {

  Config config;
  cmp::options_description desc("Options");

  for (auto str : options) {
    config.insert({str, " "});
  }

  desc.add_options()
      ("help,h", "Показать справку")
      ("scan,s", cmp::value<std::string>()->default_value(" "), "директории для сканирования")
      ("exc,e", cmp::value<std::string>()->default_value(" "), "директории для исключения из сканирования")
      ("level,l", cmp::value<std::string>()->default_value("1"), "уровень сканирования (1 на все директории 0 только указанная)")
      ("minSize,m", cmp::value<std::string>()->default_value("1"), "минимальный размер файла в байтах")
      ("blok,b", cmp::value<std::string>()->default_value("5"), "размер блока, которым производится чтения файлов");

  cmp::variables_map vm;
  cmp::store(cmp::parse_command_line(argc, argv, desc), vm);
  cmp::notify(vm);

  for (auto p : options) {
    if (vm.count(p)) {
      config[p] = vm[p].as<std::string>();
    }
  }

  if (vm.count("help")){
    std::cout << desc << std::endl;
    exit(0);
  }
  return config;
}

auto run(const Config &config) -> std::vector<fs::path> {

  auto poolFile = detail::getFileName(config);
  auto size = std::stoul(config.find("blok")->second);

  std::vector<fs::path> resuld;

  for (const auto &s : poolFile) {
    for (const auto &i : poolFile) {
      if (s != i) {
        if (detail::compare(s, i, size)) {
          if (std::find(resuld.begin(), resuld.end(), s) == resuld.end()) {
            resuld.push_back(s);
          }
           if (std::find(resuld.begin(), resuld.end(), i) == resuld.end()) {
            resuld.push_back(i);
            resuld.push_back("\n");
          }
        }
      }
    }
  }

  return resuld;
}
