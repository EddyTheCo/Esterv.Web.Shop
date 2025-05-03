#pragma once

#include <QByteArray>
#include <boost/asio.hpp>
#include <boost/beast.hpp>
namespace TCP {

constexpr std::size_t READ_BUFFER_SIZE = 1024;
constexpr std::size_t REQUEST_MAX_SIZE = 8096;
constexpr std::size_t PACKET_BUFFER_SIZE = REQUEST_MAX_SIZE + READ_BUFFER_SIZE;

class Session : public std::enable_shared_from_this<Session> {
  boost::asio::ip::tcp::socket socket_;
  std::array<uint8_t, READ_BUFFER_SIZE> read_buffer_;
  enum class ReadingState { Size, Data };
  ReadingState reading_state_{ReadingState::Size};
  uint32_t request_size_{0};
  std::size_t bytes_read_{0};
  std::array<uint8_t, PACKET_BUFFER_SIZE> packet_;
  explicit Session(boost::asio::ip::tcp::tcp::socket socket);
  void do_write(const std::vector<uint8_t> &packet_data);
  void parse_read(const std::size_t lenght);
  void parse_request(QByteArray request);
  void do_read();
  void start();

public:
  Session(const Session &) = delete;
  Session &operator=(const Session &) = delete;
  friend class Server;
};
} // namespace TCP
