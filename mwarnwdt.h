#ifndef MWARNWDT_H
#define MWARNWDT_H

#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QHash>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTreeWidgetItem>
#include <QVector>
#include <QWidget>
#include <bitset>

#include "mportmanager.h"

namespace Ui {
class mwarnwdt;
}
struct _warn_item {
  QDoubleSpinBox *VItem;
  QCheckBox *CItem;
};
class mwarnwdt : public QWidget {
  Q_OBJECT

 public:
  explicit mwarnwdt(QWidget *parent = nullptr);
  ~mwarnwdt();

 protected:
  void init();

  bool updataGUI(QStringView cmd, const queryInfo &info, int a);
  void removeItem(QTreeWidgetItem *item);
  void paintEvent(QPaintEvent *event);
 protected slots:
  void receiveData(QString, queryInfo);
  void receiveQuery(QString, queryInfo, int);
  void clickCheck(bool);
 protected slots:
  void treeItemChanged(QTreeWidgetItem *item, int column);
  void makeBits(const queryInfo &info);
  void slotClicked(QTreeWidgetItem *item, int column);
  void iChanged(QTreeWidgetItem *item);
  void logRoleChanged(uint8_t id);
  void sendCmd(int index, bool &ischeck);

 private:
  void updateItem(QTableWidget *pTable, QStringView title,
                  const uint32_t &start, const QVector<uint16_t> &val,
                  QStringView unit, QHash<uint8_t, _warn_item> &bak,
                  int treeIndex = 0);

  void updateItem(QTableWidget *pTable, QStringView title,
                  const uint32_t &start, const uint16_t &val, QStringView unit,
                  QHash<uint8_t, _warn_item> &bak, int itemIndex, int hashIndex,
                  bool &isInit);

  void queryData1(const queryInfo &info);
  void queryData2(const queryInfo &info);
  void enableTree(bool isEnable);
  inline void tableInit(QTableWidget *pTable);

 private:
  Ui::mwarnwdt *ui;
  QTreeWidgetItem *mTopSemp;
  QTreeWidgetItem *mTopJtSemp;
  QTreeWidgetItem *mTopSl;
  QTreeWidgetItem *mTopQtSemp;

  QHash<uint8_t, _warn_item> mSemp;
  QHash<uint8_t, _warn_item> mJtSemp;
  QHash<uint8_t, _warn_item> mSl;
  QHash<uint8_t, _warn_item> mQt;

  double mInt = 0;
  std::bitset<24> mBits;
  std::bitset<1> mBit2;
  uint8_t mSwitch1;
  uint8_t mSwitch2;
  uint8_t mSwitch3;
  std::atomic_bool mIsRoleChange;
  std::atomic_bool mBitsChange;
  std::atomic_bool mBitsChange2;
  std::atomic_bool mIsSendOver[2];
  bool isInt1 = 0;
  bool isInt2 = 0;
  bool isCh = true;

  QHash<QTreeWidgetItem *, bool> mItems;
  QHash<QTreeWidgetItem *, bool> mItems2;
  QVector<uint8_t> mWarn;

  uint8_t mRoleId = 0;
};

#endif  // MWARNWDT_H
