#include "meletdlgadmin.h"

#include <QPainter>
#include <QStyleOption>

#include "mlogindlg.h"
#include "mportmanager.h"
#include "ui_meletdlgadmin.h"
mEletDlgAdmin::mEletDlgAdmin(QWidget *parent)
  : QDialog(parent), ui(new Ui::mEletDlgAdmin) {
  ui->setupUi(this);
  init();
}

mEletDlgAdmin::~mEletDlgAdmin() { delete ui; }

bool mEletDlgAdmin::init() {
  // cmdData cmd;
  uint32_t cmd;
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
  //  QObject::connect(loginDlg, SIGNAL(logRoleChanged(uint8_t)), this,
  //                   SLOT(changeRose(uint8_t)));
  // 普通用户
  mStatusEdt.push_back(ui->mElecEdt1);
  mSpin1.push_back(ui->mDSpin1);
  mSpin2.push_back(ui->mDSpin1_2);
  mSpin3.push_back(ui->mDSpin1_3);

  cmd = DL_SET1;
  ui->mDSpin1_3->setProperty("cmd", cmd);
  cmd = LD1_SWITCH;
  ui->mElecEdt1->setProperty("cmd", cmd);

  mStatusEdt.push_back(ui->mElecEdt2);
  mSpin1.push_back(ui->mDSpin2);
  mSpin2.push_back(ui->mDSpin2_2);
  mSpin3.push_back(ui->mDSpin2_3);
  cmd = LD2_SWITCH;
  ui->mElecEdt2->setProperty("cmd", cmd);

  cmd = DL_SET2;
  ui->mDSpin2_3->setProperty("cmd", cmd);

  mStatusEdt.push_back(ui->mElecEdt3);
  mSpin1.push_back(ui->mDSpin3);
  mSpin2.push_back(ui->mDSpin3_2);
  mSpin3.push_back(ui->mDSpin3_3);
  cmd = LD3_SWITCH;
  ui->mElecEdt3->setProperty("cmd", cmd);
  cmd = DL_SET3;
  ui->mDSpin3_3->setProperty("cmd", cmd);

  mStatusEdt.push_back(ui->mElecEdt4);
  mSpin1.push_back(ui->mDSpin4);
  mSpin2.push_back(ui->mDSpin4_2);
  mSpin3.push_back(ui->mDSpin4_3);
  cmd = DL_SET4;
  ui->mDSpin4_3->setProperty("cmd", cmd);
  cmd = LD4_SWITCH;
  ui->mElecEdt4->setProperty("cmd", cmd);
  mStatusEdt.push_back(ui->mElecEdt5);
  mSpin1.push_back(ui->mDSpin5);
  mSpin2.push_back(ui->mDSpin5_2);
  mSpin3.push_back(ui->mDSpin5_3);
  cmd = DL_SET5;
  ui->mDSpin5_3->setProperty("cmd", cmd);
  cmd = LD5_SWITCH;
  ui->mElecEdt5->setProperty("cmd", cmd);
  for (auto i : mStatusEdt) {
    i->setProperty("status", mStatus);
    i->setStyle(QApplication::style());
  }
  // ui->displayWdt->setCurrentIndex(1);
  return true;
}

void mEletDlgAdmin::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  opt.init(this);
#else
  opt.initFrom(this);
#endif
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
void mEletDlgAdmin::receiveQuery(QString name, queryInfo info, int a) {
  updataHome(name, info, a);
}
bool mEletDlgAdmin::updataHome(QStringView name, const queryInfo &info, int a) {
  if (a != 0) return true;

  double val;
  auto s = info.DL_set.size();

  for (auto i = 0; i < s; i++) {
    int c = info.DL_Status[i];
    c &= 0x01;
    bool cc = c;
    if (mStatusEdt[i]->isChecked() != cc) {
      mStatusEdt[i]->setChecked(cc);
      mStatusEdt[i]->setProperty("status", cc);
      // mStatusEdt[i]->setStyle(QApplication::style());
    }

    val = info.DL_set[i] * 0.01;
    if (qAbs(val - mSpin1[i]->value()) >= 0.001 || mReflush) {
      mSpin1[i]->setValue(val);
      mSpin3[i]->setValue(val);
      // mReflush = false;
    }

    val = info.DL_GZ[i] * 0.1;
    if (val != mSpin2[i]->value()) mSpin2[i]->setValue(val);
  }

  return true;
}

void mEletDlgAdmin::on_mElcOk_clicked() {
  uint16_t a = 100;

  for (auto i = 0; i < mSpin2.size(); i++) {
    double d1 = mSpin1[i]->value();
    double d3 = mSpin3[i]->value();
    if (d1 == d3) continue;
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);
    uint32_t c = mSpin3[i]->property("cmd").toUInt();
    uint16_t d = round(d3 * 100);
    packet << d;
    mportMg->send(c, val);
  }
}

void mEletDlgAdmin::on_mResetBtn_2_clicked() {
  mReflush = true;
  QTimer::singleShot(1200, [&]() { mReflush = false; });

  mportMg->refresh();
}

void mEletDlgAdmin::on_mElecEdt1_clicked(bool checked) {
  sender()->blockSignals(true);
  QCheckBox *p = qobject_cast<QCheckBox *>(sender());
  p->setChecked(checked);
  uint32_t cmd = p->property("cmd").toUInt();

  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << checked;
  mportMg->send(cmd, val);
  sender()->blockSignals(false);
}

void mEletDlgAdmin::on_mElecEdt2_clicked(bool checked) {
  sender()->blockSignals(true);
  QCheckBox *p = qobject_cast<QCheckBox *>(sender());
  p->setChecked(checked);
  uint32_t cmd = p->property("cmd").toUInt();

  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << checked;
  mportMg->send(cmd, val);
  sender()->blockSignals(false);
}

void mEletDlgAdmin::on_mElecEdt3_clicked(bool checked) {
  sender()->blockSignals(true);
  QCheckBox *p = qobject_cast<QCheckBox *>(sender());
  p->setChecked(checked);
  uint32_t cmd = p->property("cmd").toUInt();

  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << checked;
  mportMg->send(cmd, val);
  sender()->blockSignals(false);
}

void mEletDlgAdmin::on_mElecEdt4_clicked(bool checked) {
  sender()->blockSignals(true);
  QCheckBox *p = qobject_cast<QCheckBox *>(sender());
  p->setChecked(checked);
  uint32_t cmd = p->property("cmd").toUInt();

  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << checked;
  mportMg->send(cmd, val);
  sender()->blockSignals(false);
}

void mEletDlgAdmin::on_mElecEdt5_clicked(bool checked) {
  sender()->blockSignals(true);
  QCheckBox *p = qobject_cast<QCheckBox *>(sender());
  p->setChecked(checked);
  uint32_t cmd = p->property("cmd").toUInt();

  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << checked;
  mportMg->send(cmd, val);
  sender()->blockSignals(false);
}
