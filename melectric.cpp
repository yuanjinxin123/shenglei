#include "melectric.h"

#include <QPainter>
#include <QStyleOption>

#include "mlogindlg.h"
#include "ui_melectric.h"

// const QString m_green_SheetStyle = R"(border-radius: 8px;
//    border : 0px;
//    background : green )";

// const QString m_grey_SheetStyle = R"(border-radius: 8px;
//    border : 0px;
//    background :grey)";
// const QString g_color[] = {m_grey_SheetStyle, m_green_SheetStyle};

mElectric::mElectric(QWidget* parent)
    : QWidget(parent), ui(new Ui::mElectric), mStatus(false) {
  ui->setupUi(this);
  init();
  setProperty("icon", ":/img/dl.png");
}

mElectric::~mElectric() { delete ui; }
void mElectric::paintEvent(QPaintEvent* event) {
  QStyleOption opt;
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
  opt.init(this);
#else
  opt.initFrom(this);
#endif
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
bool mElectric::init() {
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
  QObject::connect(loginDlg, SIGNAL(logRoleChanged(uint8_t)), this,
                   SLOT(changeRose(uint8_t)));
  // 普通用户
  mEditVer1.push_back(ui->mElecEdt1);
  mEditVer2.push_back(ui->mElecEdt1_2);
  mEditVer3.push_back(ui->mElecEdt1_3);

  mEditVer1.push_back(ui->mElecEdt2);
  mEditVer2.push_back(ui->mElecEdt2_2);
  mEditVer3.push_back(ui->mElecEdt2_3);

  mEditVer1.push_back(ui->mElecEdt3);
  mEditVer2.push_back(ui->mElecEdt3_2);
  mEditVer3.push_back(ui->mElecEdt3_3);

  mEditVer1.push_back(ui->mElecEdt4);
  mEditVer2.push_back(ui->mElecEdt4_2);
  mEditVer3.push_back(ui->mElecEdt4_3);

  mEditVer1.push_back(ui->mElecEdt5);
  mEditVer2.push_back(ui->mElecEdt5_2);
  mEditVer3.push_back(ui->mElecEdt5_3);

  for (auto i : mEditVer1) {
    i->setProperty("status", mStatus);
    i->setStyle(QApplication::style());
  }
  ui->displayWdt->setCurrentIndex(0);
  return true;
}

void mElectric::on_pushButton_clicked() {}

void mElectric::receiveQuery(QString name, queryInfo info, int a) {
  updataHome(name, info, a);
}
bool mElectric::updataHome(QStringView name, const queryInfo& info, int a) {
  if (a != 0) return true;
  // ui->displayWdt->setCurrentIndex(0);
  double val;
  auto s = info.DL_set.size();
  //    // int i = 0;
  for (auto i = 0; i < s; i++) {
    int c = info.DL_Status[i];
    c &= 0x01;
    bool cc = c;
    mEditVer1[i]->setChecked(cc);
    mEditVer1[i]->setProperty("status", cc);
    mEditVer1[i]->setStyle(QApplication::style());

    val = info.DL_set[i] * 0.01;
    mEditVer2[i]->setText(QString::number(val, 'f', 2));
    val = info.DL_GZ[i] * 0.1;
    mEditVer3[i]->setText(QString::number(val, 'f', 2));
  }

  return true;
}

void mElectric::on_mSetBtn_clicked() {}

void mElectric::changeRose(uint8_t id) {
  if (id == 0) {
    ui->displayWdt->setCurrentIndex(0);
  } else {
    ui->displayWdt->setCurrentIndex(1);
  }
}
