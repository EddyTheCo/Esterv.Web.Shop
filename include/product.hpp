#pragma once

#include "object_core.hpp"

#include <QObject>
enum ProductType : quint8 { Basic };

class Product : virtual public CoreBase<ProductType, High>, public QObject
{
    quint64 m_price;
    QString m_name;
    QString m_description;

protected:
    Product(const quint64 price, const QString name, const QString description)
        : CoreBase{ProductType::Basic}
        , m_price{price}
        , m_name{name}
        , m_description{description}
    {
    };
    Product(QByteArray id, const quint64 price, const QString name, const QString description)
        : CoreBase(ProductType::Basic, id)
        , m_price{price}
        , m_name{name}
        , m_description{description} {};

    Product(const QJsonValue &val)
        : m_price{val.toObject()["price"].toString().toULongLong()}
        , m_name{val.toObject()["name"].toString()}
        , m_description{val.toObject()["description"].toString()}
        , CoreBase(ProductType::Basic, val)
    {}
    Product(QDataStream &in)
        : CoreBase(ProductType::Basic, in)
    {
        in >> m_price;
        in >> m_name;
        in >> m_description;
    }

    void addJson(QJsonObject &var) const override
    {
        CoreBase::addJson(var);
        var.insert("price", QString::number(m_price));
        var.insert("name", m_name);
        var.insert("description", m_description);
    }
    void serialize(QDataStream &out) const override
    {
        CoreBase::serialize(out);
        out << m_price;
        out << m_name;
        out << m_description;
    }

public:
    template<typename T>
    static std::shared_ptr<Product> from(T &val)
    {
        const auto type = CoreBase::getType(val);
        switch (type) {
        case ProductType::Basic:
            return std::shared_ptr<Product>{new Product(val)};
        default:
            return nullptr;
        }
    }
    [[nodiscard]] auto price() const { return m_price; }
    [[nodiscard]] auto name() const { return m_name; }
    [[nodiscard]] auto description() const { return m_description; }

    void setPrice(const quint64 price)
    {
        if (price != m_price) {
            m_price = price;
        }
    }
    void setName(const QString name)
    {
        if (name != m_name) {
            m_name = name;
        }
    }
    void setDescription(const QString description)
    {
        if (description != m_description) {
            m_description = description;
        }
    }

    [[nodiscard]] static std::shared_ptr<Product> Basic(const quint64 price = 0,
                                                        const QString name = "",
                                                        const QString description = "");
};

class BasicProduct : public Product
{
protected:
    BasicProduct(const quint64 price, const QString name, const QString description)
        : Product(price, name, description)
        , CoreBase(ProductType::Basic)
    {
    }
    BasicProduct(QByteArray id, const quint64 price, const QString name, const QString description)
        : Product(price, name, description)
        , CoreBase(ProductType::Basic, id)
    {}

    BasicProduct(const QJsonValue &val)
        : Product(val)
        , CoreBase(ProductType::Basic, val)
    {}
    BasicProduct(QDataStream &in)
        : Product(in)
        , CoreBase(ProductType::Basic, in)
    {}
    friend class Product;
};
