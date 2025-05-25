#pragma once

#include <QByteArray>
#include "boost/asio/ip/tcp.hpp"
#include "server.hpp"
#include "socket_buffer.hpp"

#include <cstdint>
#include <memory>

namespace TCP {

class Session : public SocketBuffer, std::enable_shared_from_this<Session>
{
    std::weak_ptr<Server> server_;
    explicit Session(boost::asio::ip::tcp::tcp::socket socket, std::weak_ptr<Server> server);
    void reply(const QByteArray &reply_data);
    void parse_request(const std::vector<std::byte> &packet);
    void start();

public:
  Session(const Session &) = delete;
  auto operator=(const Session &) -> Session & = delete;
  Session(Session &&) noexcept = delete;
  auto operator=(Session &&) noexcept -> Session & = delete;
  ~Session() = default;
  friend class Server;
};
} // namespace TCP
