#include "request.hpp"

std::shared_ptr<Request> Request::Products(const RequestMethod method) {
  return std::shared_ptr<Request>{new ProductsRequest(method)};
}

template <typename T> std::shared_ptr<const Request> Request::from(T &val) {
  const auto type = CoreBase::getType(val);
  switch (type) {
  case RequestType::Products:
    return std::shared_ptr<const Request>{new ProductsRequest(val)};
  default:
    return nullptr;
  }
}
template std::shared_ptr<const Request> Request::from<QDataStream>(QDataStream &val);