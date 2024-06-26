#include "mfucwdt.h"

#include <QPainter>
#include <QStyleOption>

#include "log.h"
#include "mportmanager.h"
#include "ui_mfucwdt.h"
#include "config.h"
mFucWdt::mFucWdt(QWidget *parent)
  : QWidget(parent), ui(new Ui::mFucWdt), mIsValid(false) {
  ui->setupUi(this);

  //setAutoFillBackground(true);
  setAttribute(Qt::WA_TranslucentBackground, true);
  if (Config::getIns()->Get("main/lang").toString() == "zh") {
    ui->mResetWarnBtn->setProperty("lang", "zh");
  } else {
    ui->mResetWarnBtn->setProperty("lang", "en");
  }
  ui->mResetWarnBtn->style()->unpolish(ui->mResetWarnBtn);
  ui->mResetWarnBtn->style()->polish(ui->mResetWarnBtn);
  ui->mSwitchBtn->setProperty("run", mIsRun);
  ui->mWarnTbl->setProperty("isValid", mIsValid);
  ui->mJGZTP->setProperty("isValid", mIsValid);
  ui->mSwitchBtn->setStyle(QApplication::style());
  ui->mWarnTbl->setStyle(QApplication::style());
  ui->mJGZTP->setStyle(QApplication::style());
  ui->mJGZTP->setVisible(false);
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
  QObject::connect(mportManager::instance(), SIGNAL(DisConnect(QString)), this,
                   SLOT(DisPortConnect(QString)));
  updataStatus();
}

mFucWdt::~mFucWdt() { delete ui; }

void mFucWdt::updatePixmap(const QString &img) {
  mImg.load(img);
  QPixmap pixmap(QPixmap::fromImage(mImg));
  ui->mWarnTbl->setAutoFillBackground(true);
  ui->mWarnTbl->setScaledContents(false);
  pixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);
  ui->mWarnTbl->setPixmap(pixmap);
}

void mFucWdt::on_mSwitchBtn_clicked() {
  bool run = mIsRun;
  if (run == true)
    run = false;
  else
    run = true;

  QByteArray d;
  d.push_back(run);
  if (mportMg->send(JGQD, d) >= 0) {
    mIsRun = run;
  }
  updataStatus();
}

void mFucWdt::on_mResetWarnBtn_clicked() {
  QLOG_DEBUG() << "reset warning start";
  mportMg->send(WARNING_RESET, QByteArray());
  QLOG_DEBUG() << "reset warning end";
}
void mFucWdt::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  opt.init(this);
#else
  opt.initFrom(this);
#endif
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void mFucWdt::updataStatus() {
  ui->mSwitchBtn->setProperty("run", mIsRun);
  ui->mRunLb->setProperty("run", mIsRun);
  ui->mStopLb->setProperty("run", !mIsRun);
  ui->mWarnTbl->setProperty("isValid", mIsRun);
  ui->mJGZTP->setProperty("isValid", mIsRun);
  ui->mRunLb->setStyle(QApplication::style());
  ui->mStopLb->setStyle(QApplication::style());
  ui->mSwitchBtn->setStyle(QApplication::style());
  ui->mWarnTbl->setStyle(QApplication::style());
  ui->mJGZTP->setStyle(QApplication::style());
}

void mFucWdt::receiveQuery(QString name, queryInfo info, int a) {
  // mIsRun = true;
  if (a != 0) return;

  // if (mIsRun != (info.JG_Run & 0x01)) {
  mIsRun = info.JG_Run & 0x01;
  // QLOG_DEBUG() << "mIsRun:" << mIsRun;
  updataStatus();
  // }
}

void mFucWdt::DisPortConnect(QString name) {
  mIsRun = false;
  updataStatus();
}
