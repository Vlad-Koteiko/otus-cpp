#include "async.hpp"

namespace async {

  namespace details {

    struct CommandBlock {
      std::vector<std::string> commands;
      std::chrono::system_clock::time_point timestamp;
      size_t block_number;
    };

    struct Log {
      std::thread thread;
      std::queue<CommandBlock> queue;
      std::mutex mutex;
      std::condition_variable condition;
      std::atomic<bool> running {true};

      void run() {
        while (running || !queue.empty()) {
          CommandBlock block;
          {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock,
                           [this]() { return !queue.empty() || !running; });
          }

          if (!queue.empty()) {
            block = std::move(queue.front());
            queue.pop();
          }

          if (!block.commands.empty()) {
            std::cout << "bulk: ";
            for (size_t i = 0; i < block.commands.size(); ++i) {
              std::cout << block.commands[i];
              if (i != block.commands.size() - 1) {
                std::cout << ", ";
              }
            }
            std::cout << std::endl;
          }
        }
      }
    };

    struct FileWriter {
      std::thread thread;
      std::queue<CommandBlock> queue;
      std::mutex mutex;
      std::condition_variable condition;
      std::atomic<bool> running {true};
      int writer_id;

      FileWriter(int id) : writer_id(id) {
        thread = std::thread(&FileWriter::run, this);
      }

      ~FileWriter() {
        running = false;
        condition.notify_all();
        if (thread.joinable()) {
          thread.join();
        }
      }

      void run() {
        while (running || !queue.empty()) {
          CommandBlock block;

          {
            std::unique_lock<std::mutex> lock(mutex);
            condition.wait(lock,
                           [this]() { return !queue.empty() || !running; });
          }

          if (!queue.empty()) {
            block = std::move(queue.front());
            queue.pop();
          }

          if (!block.commands.empty()) {
            writeToFile(block);
          }
        }
      }

      void writeToFile(const CommandBlock& block) {
        std::time_t time =
            std::chrono::system_clock::to_time_t(block.timestamp);

        auto milliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                block.timestamp.time_since_epoch())
                .count()
            % 1000;

        std::string filename = "bulk" + std::to_string(time) + "_"
                               + std::to_string(milliseconds) + "_"
                               + std::to_string(writer_id) + "_"
                               + std::to_string(block.block_number) + ".log";

        std::ofstream file(filename);

        if (file.is_open()) {
          file << "bulk: ";

          for (size_t i = 0; i < block.commands.size(); ++i) {
            file << block.commands[i];
            if (i != block.commands.size() - 1) {
              file << ", ";
            }
          }

          file << std::endl;
        }
      }

      void addBlock(CommandBlock&& block) {
        {
          std::lock_guard<std::mutex> lock(mutex);
          queue.push(std::move(block));
        }
        condition.notify_one();
      }
    };

    struct Async : public Log {
      Async() {
        file_writers.push_back(std::make_unique<FileWriter>(1));
        file_writers.push_back(std::make_unique<FileWriter>(2));

        Log::thread = std::thread(&Async::Log::run, this);
      }

      ~Async() {
        file_writers.clear();
        Log::running = false;
        Log::condition.notify_all();
        if (Log::thread.joinable()) {
          Log::thread.join();
        }
      }

      Context* createContext(size_t block_size) {
        std::shared_ptr<Context> context =
            std::make_shared<Context>(block_size);
        contexts[context.get()] = context;
        return context.get();
      }

      void receive(Context* handle, std::string_view str) {
        auto context = getContext(handle);
        if (!context)
          return;
        comandParser(context, str);
      }

      void disconnect(Context* handle) {
        auto context = getContext(handle);
        if (!context)
          return;

        if (!context->current_commands.empty()) {
          flushCommands(context);
        }

        contexts.erase(handle);
      }

     private:
      void comandParser(std::shared_ptr<Context> context,
                        std::string_view str) {
        std::string command(str);

        if (command == "{") {
          if (context->brace_depth == 0 && !context->current_commands.empty()) {
            flushCommands(context);
          }
          context->brace_depth++;
          context->dynamic_block = true;
        } else if (command == "}") {
          if (context->brace_depth > 0) {
            context->brace_depth--;
            if (context->brace_depth == 0) {
              flushCommands(context);
              context->dynamic_block = false;
            }
          }
        } else {
          context->current_commands.push_back(command);

          if (!context->dynamic_block
              && context->current_commands.size() >= context->block_size) {
            flushCommands(context);
          }
        }
      }

      [[nodiscard]] std::shared_ptr<Context> getContext(
          Context* handle) noexcept {
        auto it = contexts.find(handle);
        return it != contexts.end() ? it->second : nullptr;
      }

      void flushCommands(std::shared_ptr<Context> context) {
        if (context->current_commands.empty())
          return;

        CommandBlock block;
        block.commands     = std::move(context->current_commands);
        block.timestamp    = std::chrono::system_clock::now();
        block.block_number = context->block_counter++;

        context->current_commands.clear();

        {
          std::lock_guard<std::mutex> lock(Log::mutex);
          Log::queue.push(block);
        }

        Log::condition.notify_one();

        size_t writer_index = block.block_number % file_writers.size();
        file_writers[writer_index]->addBlock(std::move(block));
      }

      std::vector<std::unique_ptr<FileWriter>> file_writers;
      std::map<Context*, std::shared_ptr<Context>> contexts;
    };

    [[nodiscard]] static Async& getHandler() {
      static Async handler;
      return handler;
    }

  }  // namespace details

  [[nodiscard]] Context* connect(size_t block_size) {
    return details::getHandler().createContext(block_size);
  }

  void receive(Context* handle, std::string_view str) {
    details::getHandler().receive(handle, str);
  }

  void disconnect(Context* handle) {
    details::getHandler().disconnect(handle);
  }
}  // namespace async
