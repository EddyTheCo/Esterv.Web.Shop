#include "tcp/client.hpp"

#include <boost/asio/connect.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/log/trivial.hpp>
#include <boost/system/detail/error_code.hpp>
#include <chrono>
#include <string>

enum : uint8_t { RETRY_SECONDS = 5, MAX_RETRY = 2 };

namespace TCP {

Client::Client(boost::asio::io_context &io_context,
               const std::string &host,
               const unsigned short port)
    : SocketBuffer(std::move(boost::asio::ip::tcp::tcp::socket(io_context)))
    , resolver_(io_context)
    , timer_(io_context)
    , host_{host}
    , port_{port}
{
    set_parse_packet_callback(
        [this](const std::vector<std::byte> &packet) { this->parse_reply(packet); });
    set_read_error_callback([this]() { this->try_connect(); });
}

void Client::connect()
{
    if (connection_state_ != ConnenctionState::Connecting) {
        connection_state_ = ConnenctionState::Connecting;
        try_connect();
    }
}

void Client::try_connect()
{
    ++connection_attempts_;
    socket().close();
    auto endpoints = resolver_.resolve(host_, std::to_string(port_));
    try {
        boost::asio::connect(socket(), endpoints);
        do_read();
    } catch (...) {
        BOOST_LOG_TRIVIAL(info) << "Failed to connect to server " << host_ << ":" << port_;
        if (connection_attempts_ < MAX_RETRY) {
            BOOST_LOG_TRIVIAL(info) << "Retrying in " << RETRY_SECONDS << " seconds";
            timer_.expires_after(std::chrono::seconds(RETRY_SECONDS));

            timer_.async_wait([this](const boost::system::error_code &error_code) {
                if (!error_code) {
                    try_connect();
                }
            });
        }
    }
}
void Client::parse_reply(const std::vector<std::byte> &packet) {}

} // namespace TCP
