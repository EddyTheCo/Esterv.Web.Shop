#include "product.hpp"
#include <gtest/gtest.h>

TEST(Product, Initialization)
{
    const auto product_1 = Product::Basic();
    EXPECT_EQ(product_1->price(), 0);
    EXPECT_EQ(product_1->name(), "");
    EXPECT_EQ(product_1->description(), "");

    const auto product_1_json = product_1->json();
    const auto product_1_bin = product_1->binary();
    qDebug() << "product_1_json:" << product_1_json;
    qDebug() << "product_1_bin:" << product_1_bin.toHex();

    const auto product_2 = Product::Basic(10, "Bath Gel", "Bath Gel to bath");
    EXPECT_EQ(product_2->price(), 10);
    EXPECT_EQ(product_2->name(), "Bath Gel");
    EXPECT_EQ(product_2->description(), "Bath Gel to bath");

    const auto product_2_json = product_2->json();
    const auto product_2_bin = product_2->binary();
    qDebug() << "product_2_json:" << product_2_json;
    qDebug() << "product_2_bin:" << product_2_bin.toHex();
}

TEST(Product, Serialization)
{
    const auto product = Product::Basic(10, "Bath Gel", "Bath Gel to bath");
    const auto product_json = product->json();
    auto product_bin = product->binary();
    qDebug() << "product_json:" << product_json;
    qDebug() << "product_bin:" << product_bin.toHex();

    const auto product_copy_from_json = Product::from(product_json);
    const auto product_copy_from_json_json = product_copy_from_json->json();
    const auto product_copy_from_json_bin = product_copy_from_json->binary();
    qDebug() << "product_copy_from_json_json:" << product_copy_from_json_json;
    qDebug() << "product_copy_from_json_bin:" << product_copy_from_json_bin.toHex();

    EXPECT_EQ(product_json, product_copy_from_json_json);
    EXPECT_EQ(product_bin, product_copy_from_json_bin);

    auto buffer = QDataStream(&product_bin, QIODevice::ReadOnly);
    const auto product_copy_from_bin = Product::from(buffer);
    const auto product_copy_from_bin_json = product_copy_from_bin->json();
    const auto product_copy_from_bin_bin = product_copy_from_bin->binary();
    qDebug() << "product_copy_from_bin_json:" << product_copy_from_bin_json;
    qDebug() << "product_copy_from_bin_bin:" << product_copy_from_bin_bin.toHex();

    EXPECT_EQ(product_json, product_copy_from_bin_json);
    EXPECT_EQ(product_bin, product_copy_from_bin_bin);
}
