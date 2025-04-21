#include "product.hpp"

std::shared_ptr<Product> Product::Basic(const quint64 price, const QString name,
                                        const QString description) {
  return std::shared_ptr<Product>{new BasicProduct(price, name, description)};
}
