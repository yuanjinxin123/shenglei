#include "mpsobasewdt.h"

#include "ui_mpsobasewdt.h"
mPsoBaseWdt::mPsoBaseWdt(QWidget *parent)
  : QWidget(parent), ui(new Ui::mPsoBaseWdt) {
  ui->setupUi(this);
  init();
}

mPsoBaseWdt::~mPsoBaseWdt() {
  mEditVec.clear();
  delete ui;
}
void mPsoBaseWdt::receiveQuery(QString name, queryInfo info, int a) {
  updataHome(name, info, a);
}

void mPsoBaseWdt::spinFinish() {
  QSpinBox *pItem = (QSpinBox *)sender();

  uint16_t v = pItem->value();
  uint16_t cmd = pItem->property("cmd").toUInt();
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << v;
  mportMg->send(cmd, val, false, false);
}
bool mPsoBaseWdt::updataHome(QStringView name, const queryInfo &info, int a) {
  if (a == 0) {
    auto v = info.FP0;
    if (info.YS_set.size() < 5) return false;
    if (ui->mFenPin1->value() != v) ui->mFenPin1->setValue(v);
    int i = 0;
    for (auto iter : mEditVec) {
      if (iter->value() != info.YS_set[i]) iter->setValue(info.YS_set[i]);
      i++;
    }

    if (ui->mMc2Edit->value() != info.MK_set2)
      ui->mMc2Edit->setValue(info.MK_set2);
  }
  if (a == 1) {
    auto v = info.FP1;
    if (v != ui->mFenPin2->value()) ui->mFenPin2->setValue(v);

    v = info.FP2;
    if (v != ui->mFenPin3->value()) ui->mFenPin3->setValue(v);
  }
  return true;
}
bool mPsoBaseWdt::init() {
  ui->mYs1Edit->setProperty("cmd", DELAY1_SET1);

  ui->mYs2Edit->setProperty("cmd", DELAY1_SET2);
  ui->mYs3Edit->setProperty("cmd", DELAY1_SET3);
  ui->mYs4Edit->setProperty("cmd", DELAY4_SET);
  ui->mYs5Edit->setProperty("cmd", DELAY5_SET);

  ui->mMc2Edit->setProperty("cmd", MK_SET2);

  ui->mFenPin1->setProperty("cmd", F_FREQ1_SET);
  ui->mFenPin2->setProperty("cmd", F_FREQ2_SET);
  ui->mFenPin3->setProperty("cmd", F_FREQ3_SET);

  mEditVec.push_back(ui->mYs1Edit);
  mEditVec.push_back(ui->mYs2Edit);
  mEditVec.push_back(ui->mYs3Edit);
  mEditVec.push_back(ui->mYs4Edit);
  mEditVec.push_back(ui->mYs5Edit);
  for (auto it : mEditVec) {
    QObject::connect(it, &QSpinBox::editingFinished, this,
                     &mPsoBaseWdt::spinFinish);
  }
  QObject::connect(ui->mFenPin1, &QSpinBox::editingFinished, this,
                   &mPsoBaseWdt::spinFinish);
  QObject::connect(ui->mFenPin2, &QSpinBox::editingFinished, this,
                   &mPsoBaseWdt::spinFinish);
  QObject::connect(ui->mFenPin3, &QSpinBox::editingFinished, this,
                   &mPsoBaseWdt::spinFinish);

  QObject::connect(ui->mMc2Edit, &QSpinBox::editingFinished, this,
                   &mPsoBaseWdt::spinFinish);

  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));

  return true;
}
