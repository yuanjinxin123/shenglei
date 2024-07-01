#ifndef MWARMINFO_H
#define MWARMINFO_H

#include <QMap>
#include <QObject>
class mwarmInfo : public QObject {
  Q_OBJECT
 public:
  explicit mwarmInfo(QObject *parent = nullptr);
  QMap<int, QString> mWarnmap = {{0, QString(QObject::tr("no warn"))},
    {1, QObject::tr("Crystal 1 temperature too high")},
    {2, QObject::tr("Crystal 2 temperature too high")},
    {3, QObject::tr("Crystal 3 temperature too high")},
    {4, QObject::tr("store alarm")},
    {5, QObject::tr("Cavity 4 temperature too high")},
    {6, QObject::tr("Low flow of water 1")},
    {7, QObject::tr("Cavity 1 humidity too high")},
    {8, QObject::tr("Cavity 5 temperature too high")},
    {9, QObject::tr("LD 1 temperature too high")},
    {10, QObject::tr("LD 4 temperature too high")},
    {11, QObject::tr("LD 2 temperature too high")},
    {12, QObject::tr("LD 5 temperature too high")},
    {13, QObject::tr("LD 3 temperature too high")},
    {22, QObject::tr("Lid open, please contact a professional technician")},
    {23, QObject::tr("lid open alarm")},
    {24, QObject::tr("seed no lock")},
    {25, QObject::tr("water flow warn")},
    {26, QObject::tr("time warn")},
    {27, QObject::tr("Cavity 2 humidity too high")},
    {28, QObject::tr("Low flow of water 2")},
    {32, QObject::tr("Seed runtime reached, shutdown and restart")},
    {1000, QObject::tr("connect no data")},
    {1001, QObject::tr("disconnect")}
  };
 signals:
};

#endif  // MWARMINFO_H
