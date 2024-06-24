#include "mFristPage.h"

#include <QPainter>
#include <QStyleOption>

#include "QStringView"
#include "log.h"
#include "mlogindlg.h"
#include "mportmanager.h"
#include "order.h"
#include "ui_mfristpage.h"
const double a = 0.1;
mFristPage::mFristPage(QWidget *parent)
  : QWidget(parent), ui(new Ui::mFristPage) {
  ui->setupUi(this);
  setAttribute(Qt::WA_TranslucentBackground, true);
  init();

  ui->mPowerEdt->setRange(0, 10000);
}

mFristPage::~mFristPage() { delete ui; }

bool mFristPage::init() {
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
  QObject::connect(loginDlg, SIGNAL(logRoleChanged(uint8_t)), this,
                   SLOT(changeRose(uint8_t)));
  ui->mDiplayWdt->setCurrentIndex(0);
  return true;
}

bool mFristPage::updataHome(QStringView name, const queryInfo &info, int a) {
  if (loginDlg->logRole() == 0) {
    if (a != 0) return true;
    QLOG_DEBUG() << info.Power_red;
    double val = info.Power_red * 0.01;
    ui->mPowerEdt->setValue(val);
    return true;
  }
  return true;
}
void mFristPage ::resizeEvent(QResizeEvent *event) {
  QWidget::resizeEvent(event);
}

void mFristPage::paintEvent(QPaintEvent *event) {
  QStyleOption opt;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  opt.init(this);
#else
  opt.initFrom(this);
#endif
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
void mFristPage::receiveData(QString name, queryInfo cmd) {}

void mFristPage::receiveQuery(QString name, queryInfo info, int a) {
  updataHome(name, info, a);
}

void mFristPage::changeRose(uint8_t id) {
  // ui->mAdminFrist->layoutInit();
  if (id != 2) {
    ui->mDiplayWdt->setCurrentIndex(0);
  } else {
    ui->mDiplayWdt->setCurrentIndex(1);
  }
}
