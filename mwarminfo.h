#ifndef MWARMINFO_H
#define MWARMINFO_H

#include <QMap>
#include <QObject>
class mwarmInfo : public QObject {
  Q_OBJECT
 public:
  explicit mwarmInfo(QObject *parent = nullptr);
  QMap<int, QString> mWarnmap = {{0, QString(QObject::tr("System OK"))},
    {1, QObject::tr("Cry1 Temp High")},
    {2, QObject::tr("Cry2 Temp High")},
    {3, QObject::tr("Cry3 Temp High")},
    {4, QObject::tr("Eeprom Fault")},
    {5, QObject::tr("Cry4 Temp High")},
    {6, QObject::tr("Water Flow 1 Low")},
    {7, QObject::tr("Humidity 1 High")},
    {8, QObject::tr("Cry5 Temp High")},
    {9, QObject::tr("Amp1 Temp high")},
    {10, QObject::tr("Amp4 Temp high")},
    {11, QObject::tr("Amp2 Temp high")},
    {12, QObject::tr("Amp5 Temp high")},
    {13, QObject::tr("Amp3 Temp high")},
    {14, QObject::tr("Case Temp Low")},
    {15, QObject::tr("Case Temp High")},
    {16, QObject::tr("Seed Fault")},
    {17, QObject::tr("WaveLength switching alarm")},
    {22, QObject::tr("Case is open, Contact factory")},
    {23, QObject::tr("Communication Error")},
    {24, QObject::tr("Seed unlocked, Click run to reset")},
    {25, QObject::tr("Flow Switch open")},
    {26, QObject::tr("Time error")},
    {27, QObject::tr("Humidity 2 High")},
    {28, QObject::tr("WaterFlow 2 Low")},
    {32, QObject::tr("Seed runtime up, shutdown and restart")},
    {33, QObject::tr("Firmware Version seed different")},
    {34, QObject::tr("FS Motor error")},
    {1000, QObject::tr("Connect no data")},
    {1001, QObject::tr("Disconnect")}
  };
 signals:
};

#endif  // MWARMINFO_H
