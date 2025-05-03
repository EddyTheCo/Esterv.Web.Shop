#pragma once
#include <boost/asio.hpp>
#include "request.hpp"
#include "reply.hpp"

namespace TCP {
class Server
{
    boost::asio::ip::tcp::acceptor acceptor_;
    static std::function<Reply(std::shared_ptr<const Request>)> reply_callback_;
    void do_accept();

public:
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;
    explicit Server(boost::asio::io_context &io_context,
                    const std::string &address,
                    const std::string &port,
                    const std::string &assets_root);
    void set_reply_callback(std::function<Reply(std::shared_ptr<const Request>)> callback)
    {
        reply_callback_ = std::move(callback);
    }

};

} // namespace TCP
