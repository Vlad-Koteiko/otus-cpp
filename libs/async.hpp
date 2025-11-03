#ifndef ASYNC_H
#define ASYNC_H

#include <stddef.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

namespace async {

  struct Context {
    size_t block_size;
    std::vector<std::string> current_commands;
    std::chrono::system_clock::time_point start_time;
    size_t brace_depth {0};
    size_t block_counter {0};
    std::atomic<bool> dynamic_block {false};

    Context(size_t bs) : block_size(bs) {
      start_time = std::chrono::system_clock::now();
    }
  };

  [[nodiscard]] Context* connect(size_t block_size);

  void receive(Context* handle, std::string_view str);

  void disconnect(Context* handle);

}  // namespace async
#endif  // ASYNC_H
