#pragma once
#include "reply.hpp"
#include "request.hpp"
#include "socket_buffer.hpp"
#include <memory>

namespace TCP {

class Client : public SocketBuffer, std::enable_shared_from_this<Client>
{
    std::map<Request::Id, std::function<void(std::shared_ptr<const Reply>)>> pending_callbacks_;
    void parse_reply(const std::vector<std::byte> &packet);

public:
    Client(const Client &) = delete;
    auto operator=(const Client &) -> Client & = delete;
    Client(Client &&) noexcept = delete;
    auto operator=(Client &&) noexcept -> Client & = delete;
    ~Client() = default;
    explicit Client(boost::asio::io_context &io_context,
                    const std::string &host,
                    unsigned short port);

    void send_request(std::shared_ptr<const Request> request,
                      std::function<void(std::shared_ptr<const Reply>)> callback = nullptr);
};

} // namespace TCP
