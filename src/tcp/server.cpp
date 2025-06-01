#include "tcp/server.hpp"
#include "reply.hpp"
#include "request.hpp"
#include "tcp/session.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/log/trivial.hpp>
#include <boost/system/detail/error_code.hpp>
#include <memory>
#include <utility>

namespace TCP {

Server::Server(boost::asio::io_context &io_context, const unsigned short port)
    : acceptor_(io_context,
                boost::asio::ip::tcp::tcp::endpoint(boost::asio::ip::tcp::tcp::v4(), port))
    , reply_callback_{[](const std::shared_ptr<const Request> &) -> std::shared_ptr<const Reply> {
        BOOST_LOG_TRIVIAL(warning) << "TCP Server reply callback not set";
        return nullptr;
    }}
{
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
}

void Server::do_accept() {
    auto self(shared_from_this());
    acceptor_.async_accept(
        [self](boost::system::error_code error_code, boost::asio::ip::tcp::socket socket) {
            if (!error_code) {
                std::shared_ptr<Session>(new Session(std::move(socket),
                                                     std::weak_ptr<Server>(self)))
                    ->start(); // should be multithreading //
            }
            self->do_accept();
        });
}

} // namespace TCP
