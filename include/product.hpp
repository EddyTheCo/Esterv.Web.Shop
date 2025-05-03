#pragma once

#include "object_core.hpp"

enum class ProductType : quint8 { Basic };
constexpr Core::HashSecurity ProductHashSecurity = Core::High;
using ProductBase = Core::Base<ProductType, ProductHashSecurity>;

class Product : virtual public ProductBase
{
    quint64 m_price;
    QString m_name;
    QString m_description;
public:
    using Id = Core::Id<ProductHashSecurity>;
    static std::shared_ptr<Product> from(QDataStream &val);
  [[nodiscard]] auto price() const { return m_price; }
  [[nodiscard]] auto name() const { return m_name; }
  [[nodiscard]] auto description() const { return m_description; }

  void setPrice(const quint64 price) {
    if (price != m_price) {
      m_price = price;
    }
  }
  void setName(const QString name) {
    if (name != m_name) {
      m_name = name;
    }
  }
  void setDescription(const QString description) {
    if (description != m_description) {
      m_description = description;
    }
  }

  [[nodiscard]] static std::shared_ptr<Product>
  Basic(const quint64 price = 0, const QString name = "",
        const QString description = "");

protected:
  
  Product(const quint64 price, const QString name, const QString description)
      : ProductBase{ProductType::Basic}, m_price{price}, m_name{name},
        m_description{description} {};
  Product(const Id &id, const quint64 price, const QString name,
          const QString description)
      : ProductBase{ProductType::Basic, id}, m_price{price}, m_name{name},
        m_description{description} {};

  Product(QDataStream &in) : ProductBase{ProductType::Basic, in} {
    in >> m_price;
    in >> m_name;
    in >> m_description;
  }

  void serialize(QDataStream &out) const override {
    ProductBase::serialize(out);
    out << m_price;
    out << m_name;
    out << m_description;
  }
  
};

class BasicProduct : public Product {
protected:
  BasicProduct(const quint64 price, const QString name,
               const QString description)
      : Product(price, name, description), ProductBase{ProductType::Basic} {}
  BasicProduct(const Id & id, const quint64 price, const QString name,
               const QString description)
      : Product(price, name, description), ProductBase{ProductType::Basic, id} {}
  BasicProduct(QDataStream &in)
      : Product(in), ProductBase{ProductType::Basic, in} {}
  friend class Product;
};


