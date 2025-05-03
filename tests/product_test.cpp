#include "product.hpp"
#include <gtest/gtest.h>

TEST(Product, Initialization) {
  const auto product_1 = Product::Basic();
  EXPECT_EQ(product_1->price(), 0);
  EXPECT_EQ(product_1->name(), "");
  EXPECT_EQ(product_1->description(), "");

  const auto product_2 = Product::Basic(10u, "Bath Gel", "Bath Gel to bath");
  EXPECT_EQ(product_2->price(), 10u);
  EXPECT_EQ(product_2->name(), "Bath Gel");
  EXPECT_EQ(product_2->description(), "Bath Gel to bath");
}

TEST(Product, Serialization) {
  const auto product = Product::Basic(10u, "Bath Gel", "Bath Gel to bath");
  auto product_bin = product->binary();
  auto buffer = QDataStream(&product_bin, QIODevice::ReadOnly);
  const auto product_copy_from_bin = Product::from(buffer);
  const auto product_copy_from_bin_bin = product_copy_from_bin->binary();
  EXPECT_EQ(product_bin, product_copy_from_bin_bin);
}
