#pragma once

#include "object_core.hpp"


enum RequestType : quint8 { Products };

enum RequestMethod: quint8 { GET, POST};

class Request : virtual public CoreBase<RequestType, Low>
{
    RequestMethod m_method{GET};

protected:
  Request(const RequestMethod method)
      : CoreBase{RequestType::Products},m_method{method} {};

      Request(QDataStream &in) : CoreBase{RequestType::Products, in} {
    in >> m_method;
  }

  void serialize(QDataStream &out) const override {
    CoreBase::serialize(out);
    out << m_method;
  }

public:
  template <typename T> static std::shared_ptr<const Request> from(T &val);
  [[nodiscard]] auto method() const { return m_method; }
 

  [[nodiscard]] static std::shared_ptr<Request>
  Products(const RequestMethod method = GET);
};

class ProductsRequest : public Request {
protected:
ProductsRequest(const RequestMethod method)
      : Request(method), CoreBase(RequestType::Products) {}

      ProductsRequest(QDataStream &in)
      : Request(in), CoreBase(RequestType::Products, in) {}
  friend class Request;
};
