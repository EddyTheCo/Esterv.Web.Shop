#include "object_core.hpp"
#include <gtest/gtest.h>

namespace Core {
enum CarType : quint8 { Lambo, Lada, Panda };

class Car : public Base<CarType, HashSecurity::VeryLow> {
public:
  Car(CarType typ) : Base(typ){};
  Car(CarType typ, const Id<HashSecurity::VeryLow> id) : Base(typ, id){};
  Car(CarType typ, QDataStream &in) : Base(typ, in){};
};

TEST(CoreBase, Serialization) {
  const auto lambo = Car(Lambo);
  auto lambo_bin = lambo.binary();

  auto buffer = QDataStream(&lambo_bin, QIODevice::ReadOnly);
  CarType car_type;
  buffer >> car_type;
  const auto lambo_copy_from_bin = Car(car_type, buffer);
  const auto lambo_copy_from_bin_bin = lambo_copy_from_bin.binary();

  EXPECT_EQ(lambo_bin, lambo_copy_from_bin_bin);
}
} // namespace Core
