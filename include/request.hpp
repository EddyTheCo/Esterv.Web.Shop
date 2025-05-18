#pragma once

#include "object_core.hpp"
#include <memory>
#include <qtypes.h>

enum class RequestType : quint8 { Products };
constexpr Core::HashSecurity RequestHashSecurity = Core::Low;
using RequestBase = Core::Base<RequestType, RequestHashSecurity>;

class Request : virtual public RequestBase {
public:
  using RequestBase::RequestBase;

  using Id = Core::Id<RequestHashSecurity>;
  enum class Method : quint8 { GET, POST };
  static auto from(QDataStream &in_stream) -> std::shared_ptr<const Request>;
  [[nodiscard]] auto method() const { return method_; }

  [[nodiscard]] static auto
  Products(Method method = Method::GET) -> std::shared_ptr<const Request>;

private:
  Method method_{Method::GET};

protected:
  Request(const Method method)
      : RequestBase{RequestType::Products}, method_{method} {}

  Request(QDataStream &in_stream)
      : RequestBase{RequestType::Products, in_stream} {
    in_stream >> method_;
  }

  void serialize(QDataStream &out) const override {
    RequestBase::serialize(out);
    out << method_;
  }
};

class ProductsRequest : public Request {
protected:
  ProductsRequest(const Method method)
      : Request(method), RequestBase(RequestType::Products) {}

  ProductsRequest(QDataStream &in_stream)
      : Request(in_stream), RequestBase(RequestType::Products, in_stream) {}
  friend class Request;
};
