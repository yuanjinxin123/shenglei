#ifndef MACOUSTO_POD_H
#define MACOUSTO_POD_H

#include <QButtonGroup>
#include <QWidget>

#include "mportmanager.h"
namespace Ui {
class mAcousto_pod;
}

class mAcousto_pod : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool mISMode READ mode WRITE setMode NOTIFY modeChanged);
  Q_PROPERTY(
    bool mIsPodGate READ podGate WRITE setPodGate NOTIFY podGateChanged);
  Q_PROPERTY(bool mIsDebug READ hasDebug WRITE setDebug NOTIFY debugChanged);
  Q_PROPERTY(bool mIsPowerMode READ hasPowerMode WRITE setPowerMode NOTIFY
             powerModeChanged);

 public:
  explicit mAcousto_pod(QWidget *parent = nullptr);
  ~mAcousto_pod();

 public:
  bool mode() const;
  void setMode(uint m);
  bool podGate() const;

  bool hasDebug() const;
  void setDebug(bool m);

  bool hasPowerMode() const;
  void setPowerMode(bool m);

 protected:
  bool init();
  bool updataHome(QStringView name, const queryInfo &info, int a);
  void updateDebug(const bool &b);
  void updatePowerMode(const bool &b);
 signals:
  void modeChanged(bool);
  void podGateChanged(bool);
  void debugChanged(bool);
  void powerModeChanged(bool);
 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);

  void modeButtonsClicked(int id);
 private slots:
  void on_mDebunBtn_clicked();

  void on_mPowerModeBtn_clicked();

  void on_mSetBtn_clicked();

  void on_mFreqSetVal_valueChanged(int arg1);
  void updateCFMode(int);
  void on_finish();
  void setPodGate(bool m);

  void on_mBurstSetVal_valueChanged(int arg1);

  void on_mPowerSetVal_valueChanged(int arg1);

  void on_mCorre_clicked();

 private:
  Ui::mAcousto_pod *ui;
  bool mTest = false;
  bool mUpdate = true;
  bool mInitVal = true;
  bool mISMode;
  bool mIsPodGate;
  bool mIsDebug;
  bool mIsPowerMode;
  QButtonGroup *pModeGroup;
  QButtonGroup *pPodGateGroup;
  QButtonGroup *pCfGroup;
  QTimer mtimer;
};

#endif  // MACOUSTO_POD_H
