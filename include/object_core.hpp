#pragma once
#include <QByteArray>
#include <QDataStream>
#include <QIODevice>
#include <QJsonObject>
#include <QRandomGenerator>

enum HashSecurity : quint8 { VeryLow = 4, Low = 8, Medium = 12, High = 16, VeryHigh = 32 };

template <HashSecurity S> using Id = std::array<quint8, static_cast<size_t>(S)>;

template <class T, HashSecurity S> class CoreBase {
  [[nodiscard]] Id<S> generateId() const {
    const size_t idSize = static_cast<size_t>(S);
    Id<S> id;
    QRandomGenerator::global()->fillRange(
        reinterpret_cast<quint32 *>(id.data()), idSize / 4);
    return id;
  }
  [[nodiscard]] Id<S> fromQByteArray(const QByteArray &bytes) {
    constexpr size_t size = static_cast<size_t>(S);

    if (bytes.size() != size)
      return Id<S>{};

    Id<S> result;
    std::memcpy(result.data(), bytes.constData(), size);
    return result;
  }
  const std::array<quint8, static_cast<size_t>(S)> m_id;

protected:
  const T m_type;

  CoreBase(T typ)
      : m_type(typ)
      , m_id{generateId()}
  {
  }
  CoreBase(T typ, QByteArray id) : m_type(typ), m_id{fromQByteArray(id)} {}
  CoreBase(T typ, const QJsonValue &val)
      : CoreBase(typ, QByteArray::fromHex(
                          val.toObject()["id"].toString().toUtf8())) {}
  CoreBase(T typ, QDataStream &in)
      : CoreBase(typ, [&in]() {
          in.setByteOrder(QDataStream::LittleEndian);
          QByteArray bytes(static_cast<size_t>(S), Qt::Uninitialized);
          if (in.readRawData(bytes.data(), static_cast<int>(S)) !=
              static_cast<int>(S)) {
            return QByteArray();
          }
          return bytes;
        }()) {}

  virtual void serialize(QDataStream &out) const {
    out << m_type;
    const auto id_ = id();
    out.writeRawData(id_.data(), id_.size());
  }

  virtual void addJson(QJsonObject &var) const {
    var.insert("type", (int)m_type);
    var.insert("id", QString(id().toHex()));
  }
  [[nodiscard]] static T getType(const QJsonValue &val)
  {
      return ((T) val.toObject()["type"].toInt());
  }

  [[nodiscard]]
  static T getType(QDataStream &val)
  {
      T type_;
      val >> type_;
      return type_;
  }

public:
  QByteArray binary() const {
    QByteArray bin;
    auto buffer = QDataStream(&bin, QIODevice::WriteOnly | QIODevice::Append);
    buffer.setByteOrder(QDataStream::LittleEndian);
    serialize(buffer);
    return bin;
  }
  QJsonObject json() const {
    QJsonObject js;
    addJson(js);
    return js;
  }

  [[nodiscard]] auto type() const { return m_type; }

  [[nodiscard]] QByteArray id() const {
    return QByteArray(reinterpret_cast<const char *>(m_id.data()),
                      static_cast<size_t>(S));
  }
};
