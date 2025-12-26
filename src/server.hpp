#ifndef SERVER_HPP
#define SERVER_HPP


#include <boost/asio.hpp>
#include <cstdlib>
#include <memory>
#include <utility>

import commander;

namespace server {

  using boost::asio::ip::tcp;

  namespace details {

    struct Session : public std::enable_shared_from_this<Session> {
      Session(tcp::socket socket, Boss& b)
          : socket_(std::move(socket))
          , boss(b) {}

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
                auto p = boss.run({data_, length});
                for (const auto& str : p.value()) {
                  do_write(str);
                  do_write("\n\r");
                }
              }
            });
      }

      void do_write(const std::string& str) {
        auto self(shared_from_this());
        boost::asio::async_write(
            socket_,
            boost::asio::buffer(str.data(), str.length()),
            [self](boost::system::error_code ec, std::size_t) {
              if (!ec) {
              }
            });
      }

      tcp::socket socket_;
      Boss& boss;
      enum { max_length = 1024 };
      char data_[max_length];
    };
  }  // namespace details

  struct Server {
    Server(short port)
        : io_context()
        , acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
      do_accept();
    }
    void run() { io_context.run(); }

    ~Server() {}

   private:
    void do_accept() {
      acceptor.async_accept([this](boost::system::error_code ec,
                                   tcp::socket socket) {
        if (!ec) {
          std::make_shared<details::Session>(std::move(socket), boss)->start();
        }
        do_accept();
      });
    }

    boost::asio::io_context io_context;
    tcp::acceptor acceptor;
    Boss boss;
  };

}  // namespace server
#endif  // ASYNC_H
