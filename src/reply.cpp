#include "reply.hpp"

std::shared_ptr<const Reply>
Reply::Products(const Request::Id &request_id,
                const std::vector<Product::Id> &products_ids) {
  return std::shared_ptr<Reply>{new ProductsReply(request_id, products_ids)};
}

std::shared_ptr<const Reply> Reply::from(QDataStream &in) {
  const auto type = ReplyBase::getType(in);
  std::shared_ptr<const Reply> result;
  switch (type) {
  case ReplyType::Products:
    result = std::shared_ptr<const Reply>{new ProductsReply(in)};
    break;
  default:
    return nullptr;
  }
  return (in.status() == QDataStream::Ok) ? result : nullptr;
}