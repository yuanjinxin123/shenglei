#include "motherwdt.h"

#include "order.h"
#include "ui_motherwdt.h"
mOtherWdt::mOtherWdt(QWidget *parent) : mWidget(parent), ui(new Ui::mOtherWdt) {
  ui->setupUi(this);
  init();
}

mOtherWdt::~mOtherWdt() { delete ui; }

void mOtherWdt::init() {
  pDWGroup = new QButtonGroup();

  pDWGroup->addButton(ui->muW, 0);
  pDWGroup->addButton(ui->mMw, 1);

  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));

  connect(ui->mCorrectSwitch, &QCheckBox::stateChanged,
          mportManager::instance(), &mportManager::curePowerEnable);

  connect(ui->mCorrectSwitch, &QCheckBox::stateChanged,
          mportManager::instance(), &mportManager::setEnableCorr);
  connect(pDWGroup, SIGNAL(buttonClicked(int)), this, SLOT(updateDw(int)));
}

void mOtherWdt::receiveQuery(QString name, queryInfo info, int a) {
  updataparam(name, info, a);
}

void mOtherWdt::updateDw(int index) {
  index %= 2;
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << (uint8_t)index;
  mportMg->send(DANGWEI, val);
}
void mOtherWdt::updataparam(QString name, queryInfo info, int a) {
  if (a == 0) {
    uint32_t t;
    uint8_t b[4];
    for (int i = 0; i < 4; i++) {
      b[3 - i] = info.JG_run_times[i];
    }
    memcpy(&t, b, 4);
    // ui->mWorkTime->setValue(t);

    ui->mWorkPoint->setValue(info.seed_run_point);
    // v = info.seed_run_times;

    //  ui->mRunTime->setText(QString::number(v));
  }
  if (a == 1) {
    auto v = info.PowerPz;
    ui->mPowerPz->setValue(v);
    v = info.PowerC;
    ui->mPowerBt->setValue(v);

    if (info.CorrectSwitch) {
      if (ui->mCorrectSwitch->isChecked() == false)
        ui->mCorrectSwitch->setChecked(true);
    } else {
      if (ui->mCorrectSwitch->isChecked() == true)
        ui->mCorrectSwitch->setChecked(false);
    }
    bool isCheck = (info.Dangwei);
    pDWGroup->button(isCheck)->setChecked(true);
    // bool b = ui->mDangwei->isChecked();
  }
}

void mOtherWdt::on_mChangePointBtn_clicked() {
  mportMg->send(CHANGE_POINT, QByteArray());
}

void mOtherWdt::on_mCorrectSwitch_clicked(bool checked) {
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  packet << checked;

  mportMg->send(CUR_SWITCH, val);
}

void mOtherWdt::on_mDangwei_clicked(bool checked) {}

void mOtherWdt::on_mCorrectSwitch_clicked() {}
