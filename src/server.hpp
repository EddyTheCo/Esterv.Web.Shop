#pragma once
#include <boost/asio.hpp>

namespace TCP {
class Server
{
    boost::asio::ip::tcp::acceptor acceptor_;

    void do_accept();

public:
    Server(const Server &) = delete;
    Server &operator=(const Server &) = delete;
    explicit Server(boost::asio::io_context &io_context,
                    const std::string &address,
                    const std::string &port,
                    const std::string &assets_root);
};

} // namespace TCP
