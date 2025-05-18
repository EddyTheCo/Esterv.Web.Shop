#include "product.hpp"
#include <QDataStream>
#include <QString>
#include <qtypes.h>

auto Product::Basic(const quint64 price, const QString name, const QString description)
    -> std::shared_ptr<Product>
{
    return std::shared_ptr<Product>{new BasicProduct(price, name, description)};
}

auto Product::from(QDataStream &in_stream) -> std::shared_ptr<Product>
{
    const auto type = ProductBase::getType(in_stream);
    std::shared_ptr<Product> result;
    switch (type) {
    case ProductType::Basic:
        result = std::shared_ptr<Product>{new BasicProduct(in_stream)};
        break;
    default:
        return nullptr;
    }
    return (in_stream.status() == QDataStream::Ok) ? result : nullptr;
}
