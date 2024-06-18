#include "msempwdt.h"

#include <QPainter>
#include <QStyleOption>

#include "mlogindlg.h"
#include "ui_msempwdt.h"
msempwdt::msempwdt(mWidget *parent) : mWidget(parent), ui(new Ui::msempwdt) {
  ui->setupUi(this);
  init();
  ui->mTHGWorkVal_3->installEventFilter(this);
  ui->mSHGWorkVal_3->installEventFilter(this);

  setProperty("icon", ":/img/workspace_tab_param.png");
}

msempwdt::~msempwdt() { delete ui; }
void msempwdt::init() {
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
  QObject::connect(loginDlg, SIGNAL(logRoleChanged(uint8_t)), this,
                   SLOT(changeRose(uint8_t)));
}

// void msempwdt::paintEvent(QPaintEvent *event) {
//  QStyleOption opt;
//#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
//  opt.init(this);
//#else
//  opt.initFrom(this);
//#endif
//  QPainter p(this);
//  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//}

void msempwdt::receiveQuery(QString name, queryInfo info, int a) {
  updataparam(name, info, a);
}
void msempwdt::updataparam(QString name, queryInfo info, int a) {
  if (a != 0) return;
  const double y = 0.01;
  double val;
  val = info.THG_WD_set * y;
  if (val != ui->mTHGSetVal->value()) {
    ui->mTHGSetVal->setValue(val);
    ui->mTHGSetVal_2->setValue(val);
    // if (mTHGChange == false) ui->mTHGWorkVal_3->setValue(val);
  }

  if (mTHGChange == false && ui->mTHGWorkVal_3->value() != val)
    ui->mTHGWorkVal_3->setValue(val);

  if (mReflush) ui->mTHGWorkVal_3->setValue(val);
  val = info.THG_WD_work * y;
  if (val != ui->mTHGWorkVal->value()) {
    ui->mTHGWorkVal->setValue(val);
    ui->mTHGWorkVal_2->setValue(val);
  }

  val = info.SHG_WD_set * y;
  if (val != ui->mSHGSetVal->value()) {
    ui->mSHGSetVal->setValue(val);
    ui->mSHGSetVal_2->setValue(val);
  }

  if (mSHGChange == false && ui->mSHGWorkVal_3->value() != val)
    ui->mSHGWorkVal_3->setValue(val);

  if (mReflush) {
    ui->mTHGWorkVal_3->setValue(val);
    mReflush = false;
  }
  val = info.SHG_WD_work * y;
  if (val != ui->mSHGWorkVal->value()) {
    ui->mSHGWorkVal->setValue(val);
    ui->mSHGWorkVal_2->setValue(val);
  }
}

bool msempwdt::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->mTHGWorkVal_3 && event->type() == QEvent::FocusIn) {
    mTHGChange = true;
  }
  if (obj == ui->mTHGWorkVal_3 && event->type() == QEvent::FocusOut)
    mTHGChange = false;

  if (obj == ui->mSHGWorkVal_3 && event->type() == QEvent::FocusIn) {
    mSHGChange = true;
  }
  if (obj == ui->mSHGWorkVal_3 && event->type() == QEvent::FocusOut)
    mSHGChange = false;

  return QWidget::eventFilter(obj, event);
}
void msempwdt::changeRose(uint8_t id) {
  if (id == 0)
    ui->mStWdt->setCurrentIndex(0);
  else
    ui->mStWdt->setCurrentIndex(1);
}

void msempwdt::on_mSempSetBtn_clicked() {
  uint16_t t = 100;
  double th1, th2;
  double sh1, sh2;
  sh1 = ui->mSHGWorkVal->value();
  sh2 = ui->mSHGWorkVal_3->value();

  if (sh1 != sh2) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);

    uint16_t v = sh2 * t;
    packet << v;
    mportMg->send(WD_SHG, val);
  }

  th1 = ui->mTHGWorkVal->value();
  th2 = ui->mTHGWorkVal_3->value();

  if (th1 != th2) {
    QByteArray val;
    QDataStream packet(&val, QIODevice::ReadWrite);
    packet.setByteOrder(QDataStream::BigEndian);

    uint16_t v = th2 * t;
    packet << v;
    mportMg->send(WD_THG, val);
  }
}

void msempwdt::on_mResetBtn_clicked() {
  double t, s;
  t = ui->mTHGSetVal_2->value();
  s = ui->mSHGSetVal_2->value();
  ui->mTHGWorkVal_3->setValue(t);
  ui->mSHGWorkVal_3->setValue(s);
}

void msempwdt::on_mSHGWorkVal_3_valueChanged(double arg1) {
  if (!mSHGChange) return;
  uint16_t t = 100;

  double sh1, sh2;
  sh1 = ui->mSHGWorkVal->value();
  sh2 = ui->mSHGWorkVal_3->value();

  // if (sh1 != sh2) {
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);

  arg1 = arg1 * t;
  arg1 = qRound(arg1);
  uint16_t v = arg1;
  packet << v;
  mportMg->send(WD_SHG, val);
  // }
}

void msempwdt::on_mTHGWorkVal_3_valueChanged(double arg1) {
  double th1, th2;
  uint16_t t = 100;
  if (!mTHGChange) return;
  th1 = ui->mTHGWorkVal->value();
  th2 = ui->mTHGWorkVal_3->value();

  // if (th1 != th2) {
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  arg1 = arg1 * t;
  arg1 = qRound(arg1);
  uint16_t v = arg1;
  packet << v;
  mportMg->send(WD_THG, val);
  //}
}
