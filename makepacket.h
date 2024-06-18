#pragma once
#include <qbytearray.h>
#include <qdatastream.h>

#include "mportmanager.h"
template <typename T>
inline void makePacket(const T& val, const int32_t& len, QByteArray& out) {
  QDataStream packet(&out, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << val;
}