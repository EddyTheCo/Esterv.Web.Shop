#pragma once
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QJsonObject>
#include <QRandomGenerator>
#include <array>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <qtypes.h>

namespace Core {
enum HashSecurity : quint8 {
  VeryLow = 4,
  Low = 8,
  Medium = 12,
  High = 16,
  VeryHigh = 32
};

template <HashSecurity S>
class Id : public std::array<quint8, static_cast<size_t>(S)> {
  using std::array<quint8, static_cast<size_t>(S)>::array;

public:
  Id() {
      QRandomGenerator::global()->fillRange(reinterpret_cast<quint32 *>(this->data()),
                                            static_cast<int>(S) / 4);
  }
  friend auto operator<<(QDataStream &out_stream, const Id &obj) -> decltype(out_stream) &
  {
      out_stream.writeRawData(reinterpret_cast<const char *>(obj.data()), static_cast<int>(S));
      return out_stream;
  }
  friend auto operator>>(QDataStream &in_stream, Id &obj) -> decltype(in_stream) &
  {
      in_stream.readRawData(reinterpret_cast<char *>(obj.data()), static_cast<int>(S));
      return in_stream;
  }
  friend auto operator<<(std::ostream &out_stream, const Id<S> &identifier)
      -> decltype(out_stream) &
  {
      out_stream << std::hex << std::setfill('0');
      for (quint8 byte : identifier) {
          out_stream << std::setw(2) << static_cast<int>(byte);
      }
      out_stream << std::dec;
      return out_stream;
  }
};

template <class T, HashSecurity S> class Base {
    const Id<S> identifier_;
    const T type_;

public:
    Base(const Base &) = delete;
    auto operator=(const Base &) -> Base & = delete;

    Base(Base &&) = delete;
    auto operator=(Base &&) -> Base & = delete;

    [[nodiscard]] auto binary() const -> QByteArray
    {
        QByteArray bin;
        auto buffer = QDataStream(&bin, QIODevice::WriteOnly | QIODevice::Append);
        serialize(buffer);
        return bin;
    }

    [[nodiscard]] auto type() const { return type_; }

    [[nodiscard]] auto id() const { return identifier_; }

protected:
    Base(T typ)
        : type_{typ}
        , identifier_{}
    {}
    Base(T typ, const Id<S> &identifier)
        : type_{typ}
        , identifier_{identifier}
    {}
    Base(T typ, QDataStream &in_stream)
        : Base(typ, [&in_stream]() -> Id<S> {
            Id<S> identifier;
            in_stream >> identifier;
            return identifier;
        }())
    {}
    ~Base() = default;

    virtual void serialize(QDataStream &out) const
    {
        out << type_;
        out << identifier_;
    }

    [[nodiscard]]
    static auto getType(QDataStream &val) -> T
    {
        T type;
        val >> type;
        return type;
    }
};
} // namespace Core
