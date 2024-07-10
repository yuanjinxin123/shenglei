#ifndef MACOUSTO_PSO_H
#define MACOUSTO_PSO_H

#include <QButtonGroup>
#include <QWidget>

#include "mportmanager.h"
namespace Ui {
class mAcousto_pso;
}

class mAcousto_pso : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool mISMode READ mode WRITE setMode NOTIFY modeChanged);
  Q_PROPERTY(
    bool mIsPodGate READ podGate WRITE setPodGate NOTIFY podGateChanged);
  Q_PROPERTY(bool mIsDebug READ hasDebug WRITE setDebug NOTIFY debugChanged);
  Q_PROPERTY(bool mIsPowerMode READ hasPowerMode WRITE setPowerMode NOTIFY
             powerModeChanged);

 public:
  explicit mAcousto_pso(QWidget *parent = nullptr);
  ~mAcousto_pso();

 public:
  bool mode() const;
  void setMode(uint m);
  bool podGate() const;
  void setPodGate(bool m);

  bool hasDebug() const;
  void setDebug(bool m);

  bool hasDebug_2() const;
  void setDebug_2(bool m);

  bool hasPowerMode() const;
  void setPowerMode(bool m);

 protected:
  bool init();
  bool updataHome(QStringView name, const queryInfo &info, int a);

 signals:
  void modeChanged(bool);
  void podGateChanged(bool);
  void debugChanged(bool);
  void debug2Changed(bool);
  void powerModeChanged(bool);
 protected slots:
  void receiveQuery(QString name, queryInfo info, int a);

 private slots:

  void updatePowerMode(int id);
  void updateDebugMode(int id);
  void updateDebug1Mode(int id);

  void on_mSetBtn_clicked();
  void modeButtonsClicked(int id);
  void on_mFreqSetVal_valueChanged(int arg1);

  void on_mBurstSetVal_valueChanged(int arg1);

  void on_mPowerSetVal_valueChanged(int arg1);
  void on_mPowerSetVal_2_valueChanged(int arg1);

  void on_mGateFreSetVal_valueChanged(int arg1);
  void on_mGateFreSetVal_2_valueChanged(int arg1);

  void on_mCorre_clicked();

 private:
  bool mUpdate = true;
  Ui::mAcousto_pso *ui;
  bool mIsInitVal = true;
  bool mIsInitVal_2 = true;
  bool mISMode;
  bool mIsPodGate;
  bool mIsDebug;
  bool mIsDebug_2;
  bool mIsPowerMode;
  QButtonGroup *pModeGroup;
  QButtonGroup *pPowerCtlGroup;
  QButtonGroup *pDebugGroup;
  QButtonGroup *pDebugGroup1;
  // QButtonGroup *pPodGateGroup;
};

#endif  // MACOUSTO_PSO_H
