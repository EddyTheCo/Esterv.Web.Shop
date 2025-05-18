#include "server.hpp"
#include "session.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <boost/system/detail/error_code.hpp>
#include <string>

namespace TCP {

Server::Server(boost::asio::io_context &io_context, const unsigned short port)
    : acceptor_(io_context,
                boost::asio::ip::tcp::tcp::endpoint(boost::asio::ip::tcp::tcp::v4(), port))
    , reply_callback_{[](std::shared_ptr<const Request>) -> std::shared_ptr<const Reply> {
        BOOST_LOG_TRIVIAL(warning) << "TCP Server reply callback not set";
        return nullptr;
    }}
{
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    do_accept();
}

void Server::do_accept() {
    acceptor_.async_accept(
        [this](boost::system::error_code error_code, boost::asio::ip::tcp::socket socket) {
            if (!error_code) {
                std::shared_ptr<Session>(
                    new Session(std::move(socket), std::weak_ptr<Server>(shared_from_this())))
                    ->start();
            }
            do_accept();
        });
}

} // namespace TCP
