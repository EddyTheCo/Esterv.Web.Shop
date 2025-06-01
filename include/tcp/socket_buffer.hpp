#pragma once

#include <array>
#include <boost/asio/ip/tcp.hpp>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <qtypes.h>
#include <utility>
#include <vector>

namespace TCP {

constexpr std::size_t READ_BUFFER_SIZE = 1024;
constexpr std::size_t PACKET_MAX_SIZE = 8096;

class SocketBuffer
{
    boost::asio::ip::tcp::socket socket_;
    std::array<std::byte, READ_BUFFER_SIZE> read_buffer_{};
    enum class ReadingState : uint8_t { Size, Data, Error };
    ReadingState reading_state_{ReadingState::Size};
    quint32 packet_size_{0};
    std::size_t bytes_read_{0};
    std::deque<std::byte> packet_;
    using ParsePacketFunction = std::function<void(const std::vector<std::byte> &)>;
    using ReadErrorFunction = std::function<void(void)>;

    ParsePacketFunction parse_packet_callback_;
    ReadErrorFunction read_error_callback_;

protected:
    void do_write(const std::vector<std::byte> &packet_data);
    auto parse_read(std::size_t lenght) -> std::vector<std::vector<std::byte>>;
    void do_read();
    auto socket() -> auto & { return socket_; }
    void set_parse_packet_callback(ParsePacketFunction callback)
    {
        parse_packet_callback_ = std::move(callback);
    }
    void set_read_error_callback(ReadErrorFunction callback)
    {
        read_error_callback_ = std::move(callback);
    }
    virtual auto get_shared_base() -> std::shared_ptr<SocketBuffer> = 0;

public:
    SocketBuffer(const SocketBuffer &) = delete;
    auto operator=(const SocketBuffer &) -> SocketBuffer & = delete;
    SocketBuffer(SocketBuffer &&) noexcept = delete;
    auto operator=(SocketBuffer &&) noexcept -> SocketBuffer & = delete;
    ~SocketBuffer() = default;

    explicit SocketBuffer(boost::asio::ip::tcp::tcp::socket socket);
};
} // namespace TCP
