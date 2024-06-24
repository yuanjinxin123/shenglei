#ifndef MWARMINFO_H
#define MWARMINFO_H

#include <QMap>
#include <QObject>
class mwarmInfo : public QObject {
  Q_OBJECT
 public:
  explicit mwarmInfo(QObject *parent = nullptr);
  QMap<int, QString> mWarnmap = {{0, QString(QObject::tr("no warn"))},
    {1, QObject::tr("JT 1 wdg")},
    {2, QObject::tr("JT 2 wdg")},
    {3, QObject::tr("JT 3 wdg")},
    {4, QObject::tr("save w")},
    {5, QObject::tr("JT 4 wdg")},
    {6, QObject::tr("Sl 1 gd")},
    {7, QObject::tr("QT 1 sdg")},
    {8, QObject::tr("JT 5 wdg")},
    {9, QObject::tr("LD 1 wdg")},
    {10, QObject::tr("LD 4 wdg")},
    {11, QObject::tr("LD 2 wdg")},
    {12, QObject::tr("LD 5 wdg")},
    {13, QObject::tr("LD 3 wdg")},
    {22, QObject::tr("G OPEN")},
    {23, QObject::tr("KG warn")},
    {24, QObject::tr("seed no lock")},
    {25, QObject::tr("sl warn")},
    {26, QObject::tr("time warn")},
    {27, QObject::tr("QT 2 sdg")},
    {28, QObject::tr("SL2 GD")},
    {32, QObject::tr("seed dao close")},
    {1000, QObject::tr("connect no data")},
    {1001, QObject::tr("disconnect")}
  };
 signals:
};

#endif  // MWARMINFO_H
