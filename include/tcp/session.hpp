#pragma once

#include "tcp/server.hpp"
#include "tcp/socket_buffer.hpp"

#include <QByteArray>
#include <boost/asio/ip/tcp.hpp>
#include <cstddef>
#include <memory>
#include <vector>

namespace TCP {

class Session : public SocketBuffer, std::enable_shared_from_this<Session>
{
    std::weak_ptr<Server> server_;
    explicit Session(boost::asio::ip::tcp::tcp::socket socket, std::weak_ptr<Server> server);
    void reply(const QByteArray &reply_data);
    void parse_request(const std::vector<std::byte> &packet);
    void start();

protected:
    auto get_shared_base() -> std::shared_ptr<SocketBuffer> override { return shared_from_this(); }

public:
  Session(const Session &) = delete;
  auto operator=(const Session &) -> Session & = delete;
  Session(Session &&) noexcept = delete;
  auto operator=(Session &&) noexcept -> Session & = delete;
  ~Session() = default;

  friend class Server;
};
} // namespace TCP
