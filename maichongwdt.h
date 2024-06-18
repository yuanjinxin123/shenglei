#ifndef MAICHONGWDT_H
#define MAICHONGWDT_H
#include <qspinbox.h>
#include <qtablewidget.h>

#include <QWidget>

#include "mportmanager.h"

namespace Ui {
class maichongwdt;
}

class maichongwdt : public QWidget {
  Q_OBJECT

 public:
  explicit maichongwdt(QWidget *parent = nullptr);
  virtual ~maichongwdt();
 signals:
  void sendcmd(const uint32_t &cmd, const QByteArray &data);

 protected:
  bool updataHome(QStringView name, const queryInfo &info, int a);
  bool init();
  void updataTable(QTableWidget *t, const QVector<uint16_t> &v,
                   const uint32_t &cmd);
 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);
  void editFinish();
  void cmdToPortMg(const uint32_t &cmd, const QByteArray &data);

 private:
  QVector<QSpinBox *> *mTbSpin1;
  QVector<QSpinBox *> *mTbSpin2;

  QVector<QTableWidgetItem *> *mTbItem1;
  QVector<QTableWidgetItem *> *mTbItem2;

 private:
  Ui::maichongwdt *ui;
};

#endif  // MAICHONGWDT_H
