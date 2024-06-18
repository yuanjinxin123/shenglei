#include "mseedparamwdt.h"

#include "mlogindlg.h"
#include "order.h"
#include "ui_mseedparamwdt.h"
mSeedparamWdt::mSeedparamWdt(QWidget *parent)
    : mWidget(parent), ui(new Ui::mSeedparamWdt) {
  ui->setupUi(this);
  setAttribute(Qt::WA_TranslucentBackground, true);
  init();
}

mSeedparamWdt::~mSeedparamWdt() { delete ui; }

void mSeedparamWdt::init() {
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));

  mDlSet.push_back(ui->mDl1Set);
  mDlSet.push_back(ui->mDl2Set);

  mDlWork.push_back(ui->mDl1Work);
  mDlWork.push_back(ui->mDl2Work);

  mSempSet.push_back(ui->mSemp1Set);
  mSempSet.push_back(ui->mSemp2Set);
  mSempSet.push_back(ui->mSemp3Set);

  mSempWork.push_back(ui->mSemp1Work);
  mSempWork.push_back(ui->mSemp2Work);
  mSempWork.push_back(ui->mSemp3Work);
}
void mSeedparamWdt::receiveQuery(QString name, queryInfo info, int a) {
  updataparam(name, info, a);
}

void mSeedparamWdt::updataparam(QString name, queryInfo info, int a) {
  if (a != 0) return;
  for (auto i = 0; i < mDlSet.size(); i++) {
    if (mDlSet[i]->value() != info.SEED_LD_set[i])
      mDlSet[i]->setValue(info.SEED_LD_set[i]);

    if (mDlWork[i]->value() != info.Seed_LD_work[i])
      mDlWork[i]->setValue(info.Seed_LD_work[i]);
  }

  for (auto i = 0; i < mSempSet.size(); i++) {
    if (mSempSet[i]->value() != (double)info.seed_T_set[i] / 10)
      mSempSet[i]->setValue((double)info.seed_T_set[i] / 10);
    if (mSempWork[i]->value() != (double)info.seed_T_work[i] / 10)
      mSempWork[i]->setValue((double)info.seed_T_work[i] / 10);
  }
  if (mIsInit) {
    ui->mDl1Change->setValue(ui->mDl1Set->value());
    ui->mDl2Change->setValue(ui->mDl2Set->value());

    ui->mSemp3Change->setValue(ui->mSemp3Set->value());
    mIsInit = false;
  }
}

void mSeedparamWdt::on_mDl1SetBtn_clicked() {
  uint16_t val;
  val = ui->mDl1Change->value();
  QByteArray b;
  QDataStream packet(&b, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << val;
  if (mportMg->send(SEED_SET1, b) == 0) {
    ui->mDl1Change->setValue(0);
  }
}

void mSeedparamWdt::on_mDl2SetBtn_clicked() {
  uint16_t val;
  val = ui->mDl2Change->value();
  QByteArray b;
  QDataStream packet(&b, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << val;

  // b = QByteArray::fromRawData((char *)&val, sizeof(uint16_t));
  if (mportMg->send(SEED_SET2, b) == 0) {
    ui->mDl2Change->setValue(0);
  }
}

void mSeedparamWdt::on_mSemp3SetBtn_clicked() {
  uint16_t val;
  sender()->blockSignals(true);
  val = qRound(ui->mSemp3Change->value() * 10);
  QByteArray b;

  QDataStream packet(&b, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << val;

  if (mportMg->send(SEED_SET3, b) == 0) {
    ui->mSemp3Change->setValue(0);
  }
  sender()->blockSignals(false);
}
