#pragma once
#include "reply.hpp"
#include "request.hpp"
#include "tcp/socket_buffer.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/steady_timer.hpp>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace TCP {

class Client : public SocketBuffer, std::enable_shared_from_this<Client>
{
    using PendingFunction = std::function<void(std::shared_ptr<const Reply>)>;
    std::map<Request::Id, PendingFunction> pending_callbacks_;
    void parse_reply(const std::vector<std::byte> &packet);
    void try_connect();
    boost::asio::ip::tcp::resolver resolver_;
    std::string host_;
    unsigned short port_{0};
    enum class ConnenctionState : uint8_t { Disconnected, Connecting, Connected };
    ConnenctionState connection_state_{ConnenctionState::Disconnected};
    boost::asio::steady_timer timer_;
    uint8_t connection_attempts_{0};

    Client(boost::asio::io_context &io_context, const std::string &host, unsigned short port);

protected:
    auto get_shared_base() -> std::shared_ptr<SocketBuffer> override { return shared_from_this(); }

public:
    static auto create(boost::asio::io_context &io_context,
                       const std::string &host,
                       unsigned short port) -> std::shared_ptr<Client>
    {
        return std::shared_ptr<Client>(new Client(io_context, host, port));
    }

    void send_request(std::shared_ptr<const Request> request, PendingFunction callback = nullptr);
    void connect();
    void set_host(const std::string &host) { host_ = host; }
    void set_port(const unsigned short &port) { port_ = port; }
};

} // namespace TCP
