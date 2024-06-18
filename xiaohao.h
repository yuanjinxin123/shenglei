#ifndef XIAOHAO_H
#define XIAOHAO_H
#include <qtablewidget.h>
#include <qvector.h>

#include <QSpinBox>
#include <QTableWidgetItem>
#include <QWidget>

#include "mportmanager.h"

namespace Ui {
class xiaohao;
}

class xiaohao : public QWidget {
  Q_OBJECT

 public:
  explicit xiaohao(QWidget *parent = nullptr);
  virtual ~xiaohao();

 protected:
  bool init();
  void updataTable(QTableWidget *t, const QVector<uint16_t> &v,
                   const uint32_t &cmd);
 signals:
  void sendcmd(const uint32_t &cmd, const QByteArray &data);
 protected slots:
  void cmdToPortMg(const uint32_t &cmd, const QByteArray &data);

 public slots:
  void receiveQuery(QString name, queryInfo info, int a);
 protected slots:
  void editFinish();

 protected:
  bool updataHome(QStringView name, const queryInfo &info, int a);

 private:
  Ui::xiaohao *ui;
  QVector<QSpinBox *> *mXhSpin1;
  QVector<QSpinBox *> *mXhSpin2;

  QVector<QTableWidgetItem *> *mXhItem1;
  QVector<QTableWidgetItem *> *mXhItem2;
};

#endif  // XIAOHAO_H
