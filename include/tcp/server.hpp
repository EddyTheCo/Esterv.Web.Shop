#pragma once
#include "reply.hpp"
#include "request.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <functional>
#include <memory>
#include <utility>

namespace TCP {
class Server : public std::enable_shared_from_this<Server>
{
    boost::asio::ip::tcp::acceptor acceptor_;
    std::function<std::shared_ptr<const Reply>(std::shared_ptr<const Request>)> reply_callback_;
    void do_accept();
    explicit Server(boost::asio::io_context &io_context, unsigned short port);

public:
    void set_reply_callback(
        std::function<std::shared_ptr<const Reply>(std::shared_ptr<const Request>)> callback)
    {
        reply_callback_ = std::move(callback);
    }
    static auto create(boost::asio::io_context &io_context, unsigned short port)
        -> std::shared_ptr<Server>
    {
        const auto server = std::shared_ptr<Server>(new Server(io_context, port));
        server->do_accept();
        return server;
    }
  friend class Session;
};

} // namespace TCP
