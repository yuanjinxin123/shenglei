#ifndef MHIGHESTWDT_H
#define MHIGHESTWDT_H

#include <QPainter>
#include <QStyleOption>
#include <QWidget>
#include <QTabWidget>

#include "mportmanager.h"
#include "mwidget.h"
namespace Ui {
class mHighestWdt;
}

class mHighestWdt : public mWidget {
  Q_OBJECT

 public:
  explicit mHighestWdt(QWidget *parent = nullptr);
  ~mHighestWdt();
  bool eventFilter(QObject *obj, QEvent *event);

  QTabWidget *getTableWidget() const;

 protected:
  void init();
  void updataparam(QString name, queryInfo info, int a);
 private slots:
  void on_mBurstBtn_clicked();

  void on_mFreqBtn_clicked();
  void on_mFreqValueMax(int v);
  void on_mFreqValueMin(int v);
  void on_comboBox_currentTextChanged(const QString &arg1);

  void on_mBurstMax_activated(int index);

  void on_mBurstMin_activated(int index);
  void receiveQuery(QString name, queryInfo info, int a);

  void on_mFreqResetBtn_clicked();

  void on_mFreqSetBtn_clicked();

  void on_mFreqVer_activated(int index);

  void on_mFreqVer_currentIndexChanged(int index);
  void on_mDlSetBtn_clicked();

  void on_mDlResetBtn_clicked();

  void on_mFreqMin_textChanged(const QString &arg1);

  void on_mFreqMin_valueChanged(int arg1);

  void on_mFreqMax_valueChanged(int arg1);

  void on_mFreqMin_editingFinished();

  void on_mFreqMax_editingFinished();

 signals:
  void changerVer(uint8_t);

 private:
  Ui::mHighestWdt *ui;
  uint16_t mFreqMin;
  uint16_t mFreqMax;
  bool mInit = true;
  bool mMaxFreqChange = false;
  bool mMinFreqChange = false;
  bool mMaxFrepPress = false;
  bool mMinFreqPress = false;
};

#endif  // MHIGHESTWDT_H
