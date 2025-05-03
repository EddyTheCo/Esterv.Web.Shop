#include "session.hpp"
#include "server.hpp"
#include"request.hpp"
namespace TCP {

Session::Session(boost::asio::ip::tcp::tcp::socket socket)
    : socket_(std::move(socket))
{}

void Session::start()
{
    do_read();
}
void Session::do_read()
{
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(read_buffer_),
                            [this, self](boost::system::error_code ec,
                                         std::size_t bytes_transferred) {
                                if (!ec && bytes_transferred > 0) {
                                    parse_read(bytes_transferred);
                                    do_read();
                                }
                            });
}
void Session::parse_read(const std::size_t lenght)
{
    std::move(read_buffer_.begin(), read_buffer_.begin() + lenght, packet_.begin() + bytes_read_);
    bytes_read_ += lenght;

    switch (reading_state_) {
    case ReadingState::Size: {
        if (bytes_read_ >= sizeof(uint32_t)) {
            request_size_ = (packet_[0] << 24) | (packet_[1] << 16) | (packet_[2] << 8)
                            | (packet_[3]);
            if (request_size_ > 0u && request_size_ <= REQUEST_MAX_SIZE) {
                reading_state_ = ReadingState::Data;
            } else {
                //close the session//
            }
        }
    }
        case ReadingState::Data: {
            if (bytes_read_ >= sizeof(uint32_t) + request_size_) {
                bytes_read_ -= sizeof(uint32_t) + request_size_;
                QByteArray request = QByteArray::fromRawData(reinterpret_cast<const char *>(
                                                                 packet_.data() + sizeof(uint32_t)),
                                                             static_cast<int>(request_size_));
                std::move(packet_.begin() + sizeof(uint32_t) + request_size_ + 1,
                          packet_.begin() + sizeof(uint32_t) + request_size_ + 1,
                          packet_.begin());

                parse_request(request);
                reading_state_ = ReadingState::Size;
            }
        }
        }
}
void Session::parse_request(QByteArray request_data) {
    QDataStream in(&request_data, QIODevice::ReadOnly);
    auto request = Request::from(in);
    /*if(Server::reply_callback_&&request) {
        // This should be inside a trhead
        auto reply = Server::reply_callback_(request);
    }*/
}
} // namespace TCP
