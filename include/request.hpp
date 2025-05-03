#pragma once

#include "object_core.hpp"

enum class RequestType : quint8 { Products };
constexpr Core::HashSecurity RequestHashSecurity = Core::Low;
using RequestBase = Core::Base<RequestType, RequestHashSecurity>;

class Request : virtual public RequestBase {
public:
  using Id = Core::Id<RequestHashSecurity>;
  enum class Method : quint8 { GET, POST };
  static std::shared_ptr<const Request> from(QDataStream &val);
  [[nodiscard]] auto method() const { return m_method; }

  [[nodiscard]] static std::shared_ptr<const Request>
  Products(const Method method = Method::GET);
  Method m_method{Method::GET};

protected:
  Request(const Method method)
      : RequestBase{RequestType::Products}, m_method{method} {};

  Request(QDataStream &in) : RequestBase{RequestType::Products, in} {
    in >> m_method;
  }

  void serialize(QDataStream &out) const override {
    RequestBase::serialize(out);
    out << m_method;
  }
};

class ProductsRequest : public Request {
protected:
  ProductsRequest(const Method method)
      : Request(method), RequestBase(RequestType::Products) {}

  ProductsRequest(QDataStream &in)
      : Request(in), RequestBase(RequestType::Products, in) {}
  friend class Request;
};