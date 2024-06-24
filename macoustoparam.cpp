#include "macoustoparam.h"

#include "mportmanager.h"
#include "ui_macoustoparam.h"

macoustoParam::macoustoParam(QWidget *parent)
  : mWidget(parent), ui(new Ui::macoustoParam) {
  ui->setupUi(this);
  init();
  ui->macoustoWidgetTab->setWidget(ui->tabWidget);
  xh = new xiaohao;
}

macoustoParam::~macoustoParam() { delete ui; }

void macoustoParam::init() {
  connect(mportMg, &mportManager::changePsoOrPod, this,
          &macoustoParam::changVer);
}

void macoustoParam::changVer(uint8_t index) {
  if (index == 1 && ui->tabWidget->count() != 4) {
    ui->tabWidget->addTab(xh, tr("xiaohao"));
  }
  if (index == 0 && ui->tabWidget->count() >= 4) {
    ui->tabWidget->removeTab(3);
  }
}
