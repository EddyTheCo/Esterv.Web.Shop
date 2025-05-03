#include "server.hpp"
#include "session.hpp"

namespace TCP {

Server::Server(boost::asio::io_context &io_context, const std::string &address,
               const std::string &port, const std::string &assets_root)
    : acceptor_(io_context) {
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
      std::shared_ptr<Session>(new Session(std::move(socket)))->start();
    }
    do_accept();
  });
}
} // namespace TCP
