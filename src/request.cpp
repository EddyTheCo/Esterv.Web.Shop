#include "request.hpp"

std::shared_ptr<const Request> Request::Products(const Request::Method method) {
  return std::shared_ptr<Request>{new ProductsRequest(method)};
}

std::shared_ptr<const Request> Request::from(QDataStream &in) {
  const auto type = RequestBase::getType(in);
  std::shared_ptr<const Request> result;
  switch (type) {
  case RequestType::Products:
    return result = std::shared_ptr<const Request>{new ProductsRequest(in)};
    break;
  default:
    return nullptr;
  }
  return (in.status() == QDataStream::Ok) ? result : nullptr;
}
