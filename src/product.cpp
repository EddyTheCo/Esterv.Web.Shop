#include "product.hpp"

std::shared_ptr<Product> Product::Basic(const quint64 price, const QString name,
                                        const QString description) {
  return std::shared_ptr<Product>{new BasicProduct(price, name, description)};
}

std::shared_ptr<Product> Product::from(QDataStream &in) {
  const auto type = ProductBase::getType(in);
  std::shared_ptr<Product> result;
  switch (type) {
  case ProductType::Basic:
    result = std::shared_ptr<Product>{new BasicProduct(in)};
    break;
  default:
    return nullptr;
  }
  return (in.status() == QDataStream::Ok) ? result : nullptr;
}
