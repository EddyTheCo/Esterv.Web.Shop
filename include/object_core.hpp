#pragma once
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QJsonObject>
#include <QRandomGenerator>
#include <iostream>
#include <iomanip>

namespace Core
{
  enum HashSecurity : quint8 {
    VeryLow = 4,
    Low = 8,
    Medium = 12,
    High = 16,
    VeryHigh = 32
  };

  template <HashSecurity S> class Id : public std::array<quint8, static_cast<size_t>(S)>
{
  using std::array<quint8, static_cast<size_t>(S)>::array;
  public:
  Id(){
    QRandomGenerator::global()->fillRange(
        reinterpret_cast<quint32 *>(this->data()), static_cast<int>(S) / 4);
  }
  friend QDataStream &operator<<(QDataStream &out, const Id &obj){
      out.writeRawData(reinterpret_cast<const char *>(obj.data()), static_cast<int>(S));
      return out;
  }
  friend QDataStream &operator>>(QDataStream &in, Id &obj){
      in.readRawData(reinterpret_cast<char *>(obj.data()), static_cast<int>(S));
      return in;
  }
friend std::ostream &operator<<(std::ostream &os, const Id<S> &id) {
        os << std::hex << std::setfill('0');
        for (quint8 byte : id) {
            os << std::setw(2) << static_cast<int>(byte);
        }
        os << std::dec;
        return os;
    }
};

template <class T, HashSecurity S> class Base {  
  const Id<S> m_id;
 public: 
  [[nodiscard]] QByteArray binary() const {
    QByteArray bin;
    auto buffer = QDataStream(&bin, QIODevice::WriteOnly | QIODevice::Append);
    serialize(buffer);
    return bin;
  }
  
  [[nodiscard]] auto type() const { return m_type; }

  [[nodiscard]] auto id() const {
    return m_id;
  }
  
protected:
  const T m_type;
  Base(T typ) : m_type(typ), m_id{} {}
  Base(T typ, const Id<S> &id) : m_type(typ), m_id{id} {}
  Base(T typ, QDataStream &in)
        : Base(typ, [&in]() -> Id<S> {
            Id<S> id;
            in >> id;
            return id;
          }()) {}

  virtual void serialize(QDataStream &out) const {
    out << m_type;
    out << m_id;
  }

  [[nodiscard]]
  static T getType(QDataStream &val) {
    T type_;
    val >> type_;
    return type_;
  }

};
}



