#include "object_core.hpp"
#include <gtest/gtest.h>

enum CarType : quint8 { Lambo, Lada, Panda };

class Car : public CoreBase<CarType, HashSecurity::VeryLow> {
public:
  Car(CarType typ) : CoreBase(typ){};
  Car(CarType typ, QByteArray id) : CoreBase(typ, id){};
  Car(CarType typ, const QJsonObject &val)
      : CoreBase(typ, val) {};
  Car(CarType typ, QDataStream &in) : CoreBase(typ, in){};
};

TEST(CoreBase, Serialization) {
  const auto lambo = Car(Lambo);
  const auto lambo_json = lambo.json();
  auto lambo_bin = lambo.binary();
  qDebug() << "lambo_json:" << lambo_json;
  qDebug() << "lambo_bin:" << lambo_bin.toHex();

  const auto car_type_json = static_cast<CarType>(lambo_json["type"].toInt());
  const auto lambo_copy_from_json = Car(car_type_json, lambo_json);
  const auto lambo_copy_from_json_json = lambo_copy_from_json.json();
  const auto lambo_copy_from_json_bin = lambo_copy_from_json.binary();
  qDebug() << "lambo_copy_from_json_json:" << lambo_copy_from_json_json;
  qDebug() << "lambo_copy_from_json_bin:" << lambo_copy_from_json_bin.toHex();

  EXPECT_EQ(lambo_json, lambo_copy_from_json_json);
  EXPECT_EQ(lambo_bin, lambo_copy_from_json_bin);

  auto buffer = QDataStream(&lambo_bin, QIODevice::ReadOnly);
  CarType car_type;
  buffer >> car_type;
  const auto lambo_copy_from_bin = Car(car_type, buffer);
  const auto lambo_copy_from_bin_json = lambo_copy_from_bin.json();
  const auto lambo_copy_from_bin_bin = lambo_copy_from_bin.binary();
  qDebug() << "lambo_copy_from_bin_json:" << lambo_copy_from_bin_json;
  qDebug() << "lambo_copy_from_bin_bin:" << lambo_copy_from_bin_bin.toHex();

  EXPECT_EQ(lambo_json, lambo_copy_from_bin_json);
  EXPECT_EQ(lambo_bin, lambo_copy_from_bin_bin);
}
