#pragma once

#include "server.hpp"

#include <QByteArray>
#include <array>
#include <boost/asio/ip/tcp.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <qtypes.h>
#include <vector>

namespace TCP {

constexpr std::size_t READ_BUFFER_SIZE = 1024;
constexpr std::size_t REQUEST_MAX_SIZE = 8096;
constexpr std::size_t PACKET_BUFFER_SIZE = REQUEST_MAX_SIZE + READ_BUFFER_SIZE;

class Session : public std::enable_shared_from_this<Session> {
    std::weak_ptr<Server> server_;
    boost::asio::ip::tcp::socket socket_;
    std::array<uint8_t, READ_BUFFER_SIZE> read_buffer_;
    enum class ReadingState : uint8_t { Size, Data };
    ReadingState reading_state_{ReadingState::Size};
    quint32 request_size_{0};
    std::size_t bytes_read_{0};
    std::array<uint8_t, PACKET_BUFFER_SIZE> packet_;

    explicit Session(boost::asio::ip::tcp::tcp::socket socket, std::weak_ptr<Server> server);
    void do_write(const std::vector<uint8_t> &packet_data);
    void reply(const QByteArray &reply_data);
    void parse_read(std::size_t lenght);
    void parse_request(QByteArray request);
    void do_read();
    void start();

public:
  Session(const Session &) = delete;
  auto operator=(const Session &) -> Session & = delete;
  ~Session() = default;
  friend class Server;
};
} // namespace TCP
