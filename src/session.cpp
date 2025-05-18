#include "session.hpp"
#include "request.hpp"
#include "server.hpp"

#include <QDataStream>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/impl/write.hpp>
#include <boost/log/trivial.hpp>
#include <boost/system/detail/error_code.hpp>
#include <memory>
#include <qtypes.h>
#include <vector>

namespace TCP {

Session::Session(boost::asio::ip::tcp::tcp::socket socket, std::weak_ptr<Server> server)
    : socket_{std::move(socket)}
    , server_{std::move(server)}
{}

void Session::start() { do_read(); }
void Session::do_read() {
  auto self(shared_from_this());
  socket_.async_read_some(boost::asio::buffer(read_buffer_),
                          [this, self](boost::system::error_code error_code,
                                       std::size_t bytes_transferred) {
                              if (!error_code && bytes_transferred > 0) {
                                  parse_read(bytes_transferred);
                                  do_read();
                              }
                          });
}
void Session::parse_read(const std::size_t lenght) {
  std::move(read_buffer_.begin(), read_buffer_.begin() + lenght,
            packet_.begin() + bytes_read_);
  bytes_read_ += lenght;

  switch (reading_state_) {
  case ReadingState::Size: {
      if (bytes_read_ >= sizeof(quint32)) {
          QByteArray byteArray(reinterpret_cast<const char *>(packet_.data()),
                               static_cast<int>(packet_.size()));
          QDataStream stream(byteArray);
          stream >> request_size_;
          if (request_size_ > 0U && request_size_ <= REQUEST_MAX_SIZE) {
              reading_state_ = ReadingState::Data;
          } else {
              // close the session//
          }
      }
  }
  case ReadingState::Data: {
      if (bytes_read_ >= sizeof(quint32) + request_size_) {
          bytes_read_ -= sizeof(quint32) + request_size_;
          const auto request = QByteArray::fromRawData(reinterpret_cast<const char *>(
                                                           packet_.data() + sizeof(quint32)),
                                                       static_cast<int>(request_size_));
          std::move(packet_.begin() + sizeof(quint32) + request_size_ + 1,
                    packet_.begin() + sizeof(quint32) + request_size_ + 1,
                    packet_.begin());

          parse_request(request);
          reading_state_ = ReadingState::Size;
      }
  }
  }
}
void Session::parse_request(QByteArray request_data) {
    QDataStream buffer(&request_data, QIODevice::ReadOnly);

    auto request = Request::from(buffer);
    auto server = server_.lock();
    if (request && server != nullptr) {
        const auto reply_obj = server->reply_callback_(request);
        if (reply_obj != nullptr) {
            reply(reply_obj->binary());
        } else {
            // close the session//
        }
    }
}
void Session::reply(const QByteArray &reply_data)
{
    QByteArray packet;
    QDataStream stream(&packet, QIODevice::WriteOnly);
    stream << static_cast<quint32>(reply_data.size());
    packet.append(reply_data);
    const std::vector<uint8_t> packet_data(packet.begin(), packet.end());
    do_write(packet_data);
}

void Session::do_write(const std::vector<uint8_t> &packet_data)
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_,
                             boost::asio::buffer(packet_data, packet_data.size()),
                             [this, self](boost::system::error_code error_code,
                                          std::size_t /*length*/) {
                                 if (error_code) {
                                     BOOST_LOG_TRIVIAL(error) << error_code.message();
                                     socket_.close();
                                 }
                             });
}
} // namespace TCP
