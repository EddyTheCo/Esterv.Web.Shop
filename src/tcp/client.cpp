#include "client.hpp"
#include <boost/asio/connect.hpp>
namespace TCP {

Client::Client(boost::asio::io_context &io_context,
               const std::string &host,
               const unsigned short port)
    : SocketBuffer(std::move(boost::asio::ip::tcp::tcp::socket(io_context)))
{
    boost::asio::ip::tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host, std::to_string(port));
    boost::asio::connect(socket(), endpoints);
    set_parse_packet_callback(
        [this](const std::vector<std::byte> &packet) { this->parse_reply(packet); });
    do_read();
}
void Client::parse_reply(const std::vector<std::byte> &packet) {}

} // namespace TCP
