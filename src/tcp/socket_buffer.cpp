#include "tcp/socket_buffer.hpp"

#include <QByteArray>
#include <QDataStream>
#include <algorithm>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/write.hpp>
#include <boost/log/trivial.hpp>
#include <boost/system/detail/error_code.hpp>
#include <cstddef>
#include <utility>
#include <vector>

namespace TCP {

SocketBuffer::SocketBuffer(boost::asio::ip::tcp::tcp::socket socket)
    : socket_{std::move(socket)}
    , parse_packet_callback_{[](const std::vector<std::byte> &) -> void {
        BOOST_LOG_TRIVIAL(warning) << "TCP parse packet callback not set";
    }}
    , read_error_callback_{[]() -> void {}}
{}

void SocketBuffer::do_read()
{
    auto self = get_shared_base();
    socket_.async_read_some(boost::asio::buffer(read_buffer_),
                            [self](boost::system::error_code error_code,
                                   std::size_t bytes_transferred) {
                                if (!error_code && bytes_transferred > 0) {
                                    const auto packets = self->parse_read(bytes_transferred);
                                    self->do_read();
                                    for (const auto &packet : packets) {
                                        self->parse_packet_callback_(packet);
                                    }
                                } else {
                                    BOOST_LOG_TRIVIAL(error) << error_code.message();
                                    self->read_error_callback_();
                                }
                            });
}
auto SocketBuffer::parse_read(const std::size_t lenght) -> std::vector<std::vector<std::byte>>
{
    std::vector<std::vector<std::byte>> packets;
    packet_.insert(packet_.end(), read_buffer_.begin(), read_buffer_.begin() + lenght);
    bytes_read_ += lenght;

    while ((reading_state_ == ReadingState::Size && bytes_read_ >= sizeof(decltype(packet_size_)))
           || (reading_state_ == ReadingState::Data && bytes_read_ >= packet_size_)) {
        switch (reading_state_) {
        case ReadingState::Size: {
            QByteArray byteArray;
            byteArray.resize(sizeof(decltype(packet_size_)));
            std::copy(packet_.begin(),
                      packet_.begin() + sizeof(decltype(packet_size_)),
                      reinterpret_cast<std::byte *>(byteArray.data()));
            QDataStream stream(byteArray);
            stream >> packet_size_;
            bytes_read_ -= sizeof(decltype(packet_size_));
            if (packet_size_ > 0U && packet_size_ <= PACKET_MAX_SIZE) {
                reading_state_ = ReadingState::Data;
            } else {
                reading_state_ = ReadingState::Error;
            }
            break;
        }
        case ReadingState::Data: {
            bytes_read_ -= packet_size_;
            const std::vector<std::byte> packet(packet_.begin() + sizeof(decltype(packet_size_)),
                                                packet_.begin() + sizeof(decltype(packet_size_))
                                                    + packet_size_);
            packets.push_back(packet);
            packet_.erase(packet_.begin(),
                          packet_.begin() + sizeof(decltype(packet_size_)) + packet_size_);
            reading_state_ = ReadingState::Size;
            break;
        }
        case ReadingState::Error:
            break;
        }
    }
    return packets;
}
void SocketBuffer::do_write(const std::vector<std::byte> &packet_data)
{
    auto self = get_shared_base();
    boost::asio::async_write(socket_,
                             boost::asio::buffer(packet_data, packet_data.size()),
                             [self](boost::system::error_code error_code, std::size_t /*length*/) {
                                 if (error_code) {
                                     BOOST_LOG_TRIVIAL(error) << error_code.message();
                                     self->socket().close();
                                 }
                             });
}
} // namespace TCP
