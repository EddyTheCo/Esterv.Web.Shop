#include "reply.hpp"
#include <QDataStream>
#include "product.hpp"
#include "request.hpp"

#include <memory>
#include <vector>
auto Reply::Products(const Request::Id &request_id, const std::vector<Product::Id> &products_ids)
    -> std::shared_ptr<const Reply>
{
    return std::shared_ptr<Reply>{new ProductsReply(request_id, products_ids)};
}

auto Reply::from(QDataStream &in_stream) -> std::shared_ptr<const Reply>
{
    const auto type = ReplyBase::getType(in_stream);
    std::shared_ptr<const Reply> result;
    switch (type) {
    case ReplyType::Products:
        result = std::shared_ptr<const Reply>{new ProductsReply(in_stream)};
        break;
    default:
        return nullptr;
    }
    return (in_stream.status() == QDataStream::Ok) ? result : nullptr;
}
