#include <server.hpp>
#include <string>

int main([[maybe_unused]] int argc, char* argv[]) {
 
  server::Server server(std::stol(argv[1]));
  server.run();

  return 0;
}
