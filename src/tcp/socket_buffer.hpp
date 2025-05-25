#pragma once

#include <array>
#include <boost/asio/ip/tcp.hpp>
#include <cstddef>
#include <deque>
#include <memory>
#include <qtypes.h>
#include <vector>

namespace TCP {

constexpr std::size_t READ_BUFFER_SIZE = 1024;
constexpr std::size_t PACKET_MAX_SIZE = 8096;

class SocketBuffer : public std::enable_shared_from_this<SocketBuffer>
{
    boost::asio::ip::tcp::socket socket_;
    std::array<std::byte, READ_BUFFER_SIZE> read_buffer_{};
    enum class ReadingState : uint8_t { Size, Data, Error };
    ReadingState reading_state_{ReadingState::Size};
    quint32 packet_size_{0};
    std::size_t bytes_read_{0};
    std::deque<std::byte> packet_{};
    std::function<void(const std::vector<std::byte> &)> parse_packet_callback_;

protected:
    void do_write(const std::vector<std::byte> &packet_data);
    std::vector<std::vector<std::byte>> parse_read(std::size_t lenght);
    void do_read();
    auto socket() -> auto & { return socket_; }
    void set_parse_packet_callback(std::function<void(const std::vector<std::byte> &)> callback)
    {
        parse_packet_callback_ = std::move(callback);
    }

public:
    explicit SocketBuffer(boost::asio::ip::tcp::tcp::socket socket);
    SocketBuffer(const SocketBuffer &) = delete;
    auto operator=(const SocketBuffer &) -> SocketBuffer & = delete;
    SocketBuffer(SocketBuffer &&) noexcept = delete;
    auto operator=(SocketBuffer &&) noexcept -> SocketBuffer & = delete;
    ~SocketBuffer() = default;
};
} // namespace TCP
