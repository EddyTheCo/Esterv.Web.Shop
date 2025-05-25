#include "session.hpp"
#include "request.hpp"
#include "server.hpp"

#include <QDataStream>
#include <memory>
#include <qtypes.h>
#include <utility>
#include <vector>

namespace TCP {

Session::Session(boost::asio::ip::tcp::tcp::socket socket, std::weak_ptr<Server> server)
    : SocketBuffer(std::move(socket))
    , server_{std::move(server)}
{
    set_parse_packet_callback(
        [this](const std::vector<std::byte> &packet) { this->parse_request(packet); });
}

void Session::start() { do_read(); }

void Session::parse_request(const std::vector<std::byte> &packet)
{
    QByteArray request_data(reinterpret_cast<const char *>(packet.data()),
                            static_cast<int>(packet.size()));
    QDataStream buffer(&request_data, QIODevice::ReadOnly);

    auto request = Request::from(buffer);
    auto server = server_.lock();
    if (request && server != nullptr) {
        const auto reply_obj = server->reply_callback_(request);
        if (reply_obj != nullptr) {
            reply(reply_obj->binary());
        } else {
            // close the session ?//
        }
    }
}

void Session::reply(const QByteArray &reply_data) {
  QByteArray packet;
  QDataStream stream(&packet, QIODevice::WriteOnly);
  stream << static_cast<quint32>(reply_data.size());
  packet.append(reply_data);
  auto data_ptr = reinterpret_cast<const std::byte *>(packet.constData());
  const std::vector<std::byte> packet_data(data_ptr, data_ptr + packet.size());
  do_write(packet_data);
}

} // namespace TCP
