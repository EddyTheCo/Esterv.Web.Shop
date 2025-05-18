#pragma once

#include "object_core.hpp"
#include <memory>
#include <qtypes.h>

enum class ProductType : quint8 { Basic };
constexpr Core::HashSecurity ProductHashSecurity = Core::High;
using ProductBase = Core::Base<ProductType, ProductHashSecurity>;

class Product : virtual public ProductBase {
    quint64 price_{0};
    QString name_;
    QString description_;

public:
    using ProductBase::ProductBase;

    using Id = Core::Id<ProductHashSecurity>;
    static auto from(QDataStream &val) -> std::shared_ptr<Product>;
    [[nodiscard]] auto price() const { return price_; }
    [[nodiscard]] auto name() const { return name_; }
    [[nodiscard]] auto description() const { return description_; }

    void setPrice(const quint64 price)
    {
        if (price != price_) {
            price_ = price;
        }
    }
    void setName(const QString &name)
    {
        if (name != name_) {
            name_ = name;
        }
    }
    void setDescription(const QString &description)
    {
        if (description != description_) {
            description_ = description;
        }
    }

    [[nodiscard]] static auto Basic(quint64 price = 0, QString name = "", QString description = "")
        -> std::shared_ptr<Product>;

protected:
    Product(const quint64 price, const QString name, const QString description)
        : ProductBase{ProductType::Basic}
        , price_{price}
        , name_{name}
        , description_{description}
    {}
    Product(const Id &id, const quint64 price, const QString name, const QString description)
        : ProductBase{ProductType::Basic, id}
        , price_{price}
        , name_{name}
        , description_{description}
    {}

    Product(QDataStream &in_stream)
        : ProductBase{ProductType::Basic, in_stream}
    {
        in_stream >> price_;
        in_stream >> name_;
        in_stream >> description_;
    }

    void serialize(QDataStream &out) const override
    {
        ProductBase::serialize(out);
        out << price_;
        out << name_;
        out << description_;
    }
};

class BasicProduct : public Product {
protected:
    BasicProduct(const quint64 price, const QString &name, const QString &description)
        : Product(price, name, description)
        , ProductBase{ProductType::Basic}
    {}
    BasicProduct(const Id &id, const quint64 price, const QString name, const QString &description)
        : Product(price, name, description)
        , ProductBase{ProductType::Basic, id}
    {}
    BasicProduct(QDataStream &in_stream)
        : Product(in_stream)
        , ProductBase{ProductType::Basic, in_stream}
    {}
    friend class Product;
};
