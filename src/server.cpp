#include "server.hpp"
#include "session.hpp"
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <string>

namespace TCP {

Server::Server(boost::asio::io_context &io_context,
               const std::string &address,
               const std::string &port,
               const std::string &assets_root)
    : acceptor_(io_context)
    , reply_callback_{[](std::shared_ptr<const Request>) -> std::shared_ptr<const Reply> {
        BOOST_LOG_TRIVIAL(warning) << "TCP Server reply callback not set";
        return nullptr;
    }}
{
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();
    do_accept();
}

void Server::do_accept() {
  acceptor_.async_accept([this](boost::system::error_code ec,
                                boost::asio::ip::tcp::socket socket) {
    if (!ec) {
        std::shared_ptr<Session>(
            new Session(std::move(socket), std::weak_ptr<Server>(shared_from_this())))
            ->start();
    }
    do_accept();
  });
}

} // namespace TCP
