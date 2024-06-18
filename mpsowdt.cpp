#include "mpsowdt.h"

#include <QPainter>
#include <QStyleOption>

#include "mlogindlg.h"
#include "ui_mpsowdt.h"
mPsoWdt::mPsoWdt(QWidget *parent) : QWidget(parent), ui(new Ui::mPsoWdt) {
  ui->setupUi(this);
  init();
  setAttribute(Qt::WA_TranslucentBackground, true);
}

mPsoWdt::~mPsoWdt() { delete ui; }

void mPsoWdt::receiveQuery(QString name, queryInfo info, int a) {
  updataHome(name, info, a);
}
void mPsoWdt::spinFinished() {
  QSpinBox *pItem = (QSpinBox *)sender();

  uint16_t v = pItem->value();
  uint16_t cmd = pItem->property("cmd").toUInt();
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << v;
  mportMg->send(cmd, val, false, false);
}
bool mPsoWdt::updataHome(QStringView name, const queryInfo &info, int a) {
  if (a == 0) {
    if (info.YS_set.size() < 4) return false;
    if (ui->mYs1Edit->value() != info.YS_set[0])
      ui->mYs1Edit->setValue(info.YS_set[0]);

    if (ui->mYs2Edit->value() != info.YS_set[1])
      ui->mYs2Edit->setValue(info.YS_set[1]);

    if (ui->mYs4Edit->value() != info.YS_set[3])
      ui->mYs4Edit->setValue(info.YS_set[3]);
    if (ui->mMk2Edit->value() != info.MK_set2)
      ui->mMk2Edit->setValue(info.MK_set2);
  }

  return true;
}
bool mPsoWdt::init() {
  ui->mYs1Edit->setProperty("cmd", DELAY1_SET1);

  ui->mYs2Edit->setProperty("cmd", DELAY1_SET2);

  ui->mYs4Edit->setProperty("cmd", DELAY4_SET);

  ui->mMk2Edit->setProperty("cmd", MK_SET2);

  QObject::connect(ui->mYs1Edit, &QSpinBox::editingFinished, this,
                   &mPsoWdt::spinFinished);
  QObject::connect(ui->mYs2Edit, &QSpinBox::editingFinished, this,
                   &mPsoWdt::spinFinished);
  QObject::connect(ui->mYs4Edit, &QSpinBox::editingFinished, this,
                   &mPsoWdt::spinFinished);
  QObject::connect(ui->mMk2Edit, &QSpinBox::editingFinished, this,
                   &mPsoWdt::spinFinished);

  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
  QObject::connect(loginDlg, SIGNAL(logRoleChanged(uint8_t)), this,
                   SLOT(changeRose(uint8_t)));
  return true;
}
void mPsoWdt::changeRose(uint8_t id) {}
void mPsoWdt::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  opt.init(this);
#else
  opt.initFrom(this);
#endif
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void mPsoWdt::on_mYs1Edit_valueChanged(int arg1) {}
