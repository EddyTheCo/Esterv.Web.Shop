#pragma once

#include "object_core.hpp"
#include "product.hpp"
#include "request.hpp"
#include <memory>
#include <qtypes.h>
#include <vector>

enum class ReplyType : quint8 { Products };
constexpr Core::HashSecurity ReplyHashSecurity = Core::VeryLow;
using ReplyBase = Core::Base<ReplyType, ReplyHashSecurity>;

class Reply : virtual public ReplyBase {
  const Request::Id request_id_;

public:
  using ReplyBase::ReplyBase;

  using Id = Core::Id<ReplyHashSecurity>;
  static auto from(QDataStream &val) -> std::shared_ptr<const Reply>;

  [[nodiscard]] auto requestId() const { return request_id_; }
  [[nodiscard]] static auto
  Products(const Request::Id &request_id,
           const std::vector<Product::Id> &products_ids)
      -> std::shared_ptr<const Reply>;

protected:
  Reply(const Request::Id &request_id)
      : ReplyBase{ReplyType::Products}, request_id_{request_id} {}

  Reply(QDataStream &in_stream)
      : ReplyBase{ReplyType::Products, in_stream}, request_id_{[&in_stream]() {
          Request::Id identifier;
          in_stream >> identifier;
          return identifier;
        }()} {}

  void serialize(QDataStream &out) const override {
    ReplyBase::serialize(out);
    out << request_id_;
  }
};

class ProductsReply : public Reply {
  const std::vector<Product::Id> products_ids_;

protected:
  ProductsReply(const Request::Id &request_id,
                const std::vector<Product::Id> &products_ids)
      : Reply(request_id), ReplyBase(ReplyType::Products),
        products_ids_{products_ids} {}

  ProductsReply(QDataStream &in_stream)
      : Reply(in_stream), ReplyBase(ReplyType::Products, in_stream),
        products_ids_{[&in_stream]() {
          std::vector<Product::Id> products_ids;
          // only 255 products add pagination //
          quint8 count{0};
          in_stream >> count;
          products_ids.reserve(count);
          for (quint8 i = 0; i < count; ++i) {
            Product::Id identifier;
            in_stream >> identifier;
            products_ids.push_back(identifier);
          }
          return products_ids;
        }()} {}

  void serialize(QDataStream &out) const override {
    Reply::serialize(out);

    // Return a max of 255 products, need to add pagination //
    const quint8 count = products_ids_.size();
    out << count;
    for (const auto &identifier : products_ids_) {
      out << identifier;
    }
  }
  friend class Reply;
};
