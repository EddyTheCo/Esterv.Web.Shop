#include "request.hpp"
#include <QDataStream>
#include <memory>

auto Request::Products(const Request::Method method) -> std::shared_ptr<const Request>
{
    return std::shared_ptr<Request>{new ProductsRequest(method)};
}

auto Request::from(QDataStream &in_stream) -> std::shared_ptr<const Request>
{
    const auto type = RequestBase::getType(in_stream);
    std::shared_ptr<const Request> result;
    switch (type) {
    case RequestType::Products:
        return result = std::shared_ptr<const Request>{new ProductsRequest(in_stream)};
        break;
    default:
        return nullptr;
    }
    return (in_stream.status() == QDataStream::Ok) ? result : nullptr;
}
