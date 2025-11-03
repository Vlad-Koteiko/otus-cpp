#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio.hpp>
#include <cstdlib>
#include <memory>
#include <utility>

#include "async.hpp"

namespace server {

  using boost::asio::ip::tcp;

  namespace details {

    struct Session : public std::enable_shared_from_this<Session> {
      Session(tcp::socket socket, async::Context* p)
          : socket_(std::move(socket))
          , ptr(p) {}

      void start() { do_read(); }

     private:
      std::vector<std::string> splitStringByNewline(const std::string& input) {
        std::vector<std::string> lines;
        std::istringstream stream(input);
        std::string line;

        while (std::getline(stream, line)) {
          lines.push_back(line);
        }

        return lines;
      }

      void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
              if (!ec) {
                auto pool = splitStringByNewline({data_, length});
                for (auto& s : pool) {
                  receive(ptr, s);
                }
              }
            });
      }
      tcp::socket socket_;
      async::Context* ptr;
      enum { max_length = 1024 };
      char data_[max_length];
    };
  }  // namespace details

  struct Server {
    Server(short port, std::uint8_t line)
        : io_context()
        , acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
      global_ctx = async::connect(line);
      do_accept();
    }
    void run() { io_context.run(); }

    ~Server() { async::disconnect(global_ctx); }

   private:
    void do_accept() {
      acceptor.async_accept(
          [this](boost::system::error_code ec, tcp::socket socket) {
            if ((!ec) and (global_ctx != nullptr)) {
              std::make_shared<details::Session>(std::move(socket), global_ctx)
                  ->start();
            }
            do_accept();
          });
    }

    boost::asio::io_context io_context;
    tcp::acceptor acceptor;
    async::Context* global_ctx = nullptr;
  };

}  // namespace server
#endif  // ASYNC_H
