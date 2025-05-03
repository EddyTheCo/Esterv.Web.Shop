#pragma once

#include "object_core.hpp"
#include "product.hpp"
#include "request.hpp"

  enum class ReplyType : quint8 { Products };
  constexpr Core::HashSecurity ReplyHashSecurity = Core::VeryLow;
  using ReplyBase = Core::Base<ReplyType, ReplyHashSecurity>;

class Reply : virtual public ReplyBase
{
  const Request::Id m_request_id;
  public: 
  using Id = Core::Id<ReplyHashSecurity>;
  static std::shared_ptr<const Reply> from(QDataStream &val);
  
  [[nodiscard]] auto requestId() const { return m_request_id;}
  [[nodiscard]] static std::shared_ptr<const Reply>
  Products(const Request::Id &request_id, const std::vector<Product::Id> &products_ids);

protected:
Reply(const Request::Id &request_id)
      : ReplyBase{ReplyType::Products}, m_request_id{request_id} {};

      Reply(QDataStream &in) : ReplyBase{ReplyType::Products, in}, m_request_id{[&in]() {
        Request::Id id;
        in >> id;
        return id;
      }()} {
              }

  void serialize(QDataStream &out) const override {
    ReplyBase::serialize(out);
    out << m_request_id;
  }

  
  
};

class ProductsReply : public Reply {
  const std::vector<Product::Id> m_products_ids;
protected:
ProductsReply(const Request::Id &request_id,const std::vector<Product::Id> &products_ids)
      : Reply(request_id), ReplyBase(ReplyType::Products),m_products_ids{products_ids} {}

      ProductsReply(QDataStream &in)
      : Reply(in), ReplyBase(ReplyType::Products, in), m_products_ids{[&in]() {
        std::vector<Product::Id> products_ids;
        uint8_t count{0};
        in >> count;
        products_ids.reserve(count);
        for (uint8_t i = 0; i < count; ++i)
        {
            Product::Id id;
            in >> id;
            products_ids.push_back(id);
        }
        return products_ids;
      }()} {
              }

      void serialize(QDataStream &out) const override {
        Reply::serialize(out);
        
        uint8_t count = m_products_ids.size();
        out << count;
        for (const auto &id : m_products_ids)
        {
            out<<id;
        }
      }
  friend class Reply;
};


