#include "macoustobase.h"

#include "mportmanager.h"
#include "ui_macoustobase.h"
mAcoustoBase::mAcoustoBase(QWidget *parent)
  : QWidget(parent), ui(new Ui::mAcoustoBase) {
  ui->setupUi(this);
  init();
}

mAcoustoBase::~mAcoustoBase() { delete ui; }

void mAcoustoBase::init() {
  QObject::connect(mportManager::instance(), SIGNAL(changePsoOrPod(uint8_t)),
                   this, SLOT(changePsoOrPod(uint8_t)));
}
void mAcoustoBase::changePsoOrPod(uint8_t id) {
  ui->stackedWidget->setCurrentIndex(id);
}
