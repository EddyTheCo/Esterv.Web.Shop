#include "product.hpp"

std::shared_ptr<Product> Product::Basic(const quint64 price, const QString name,
                                        const QString description) {
  return std::shared_ptr<Product>{new BasicProduct(price, name, description)};
}

template <typename T> std::shared_ptr<Product> Product::from(T &val) {
  const auto type = CoreBase::getType(val);
  switch (type) {
  case ProductType::Basic:
    return std::shared_ptr<Product>{new BasicProduct(val)};
  default:
    return nullptr;
  }
}

template std::shared_ptr< Product> Product::from<const QJsonObject>(const QJsonObject &val);
template std::shared_ptr< Product> Product::from<QDataStream>(QDataStream &val);