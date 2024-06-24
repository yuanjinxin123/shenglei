#include "mpowerinfowdt.h"

#include <qlabel.h>

#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QVBoxLayout>

#include "log.h"
#include "ui_mpowerinfowdt.h"
const char c_num = 5;
mPowerInfoWdt::mPowerInfoWdt(QWidget *parent)
  : QWidget(parent), ui(new Ui::mPowerInfoWdt), mMin(0.0), mMax(100.0) {
  ui->setupUi(this);
  qApp->installEventFilter(this);
  init();
  setFocusPolicy(Qt::ClickFocus);
}

mPowerInfoWdt::~mPowerInfoWdt() {
  releaseItems();
  delete ui;
}
void mPowerInfoWdt::init() {
  mLay = new QGridLayout(this);
  queryInfo info = {};
  layoutInit(info);
  QObject::connect(mportManager::instance(),
                   SIGNAL(sendInfo(QString, queryInfo, int)), this,
                   SLOT(receiveQuery(QString, queryInfo, int)));
}

void mPowerInfoWdt::layoutInit(queryInfo info) {
  // releaseItems();
  // clearWidgets(mLay);

  if (mValItems.isEmpty() || mValItems.size() != info.Power.size()) {
    releaseItems();
    clearWidgets(mLay);
    mValItems.reserve(info.Power.size());
    auto v = mLay;
    v->setSpacing(0);
    //v->addStretch(1);
    for (auto i = 0; i < info.Power.size(); i++) {
      auto key = QString(tr("power%1")).arg(i);
      auto val = new QDoubleSpinBox(this);
      val->setMinimumHeight(45);
      val->setFocusPolicy(Qt::NoFocus);
      mValItems[key] = val;

      auto h = new QVBoxLayout();
      h->setSpacing(5);
      h->addStretch(0);
      // QString t = QString(tr("power%1")).arg(i);

      auto title = new QLabel(this);
      title->setText(key);
      title->show();
      // auto val = mValItems[t];

      // mValItems[title->text()] = val;
      val->setObjectName(QString("power_val"));
      val->setSuffix("W");

      val->setAlignment(Qt::AlignHCenter);
      val->setButtonSymbols(QDoubleSpinBox::NoButtons);
      val->setReadOnly(true);
      val->setMaximum(10000);
      if (info.Power.isEmpty() == false)
        val->setValue(info.Power[i] * 0.01);
      else
        val->setValue(0);
      h->addWidget(title);
      title->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      h->addSpacing(30);
      h->addWidget(val);
      h->addStretch();

      //v->addSpacing(20);
      if (i < 3) {
        v->addLayout(h, 0, i);
      } else {
        v->addLayout(h, 1, i - 3);
      }

      //v->addStretch(1);
    }

    //v->addStretch();

    this->setLayout(v);
    update();
    return;
  }

  for (auto i = 0; i < info.Power.size(); i++) {
    auto key = QString(tr("power%1")).arg(i);
    //  auto val = new QDoubleSpinBox(this);
    if (mValItems.contains(key) == 0) {
      mValItems.clear();
      break;
    }
    mValItems[key]->setValue(info.Power[i] * 0.01);
  }

  update();
}


void mPowerInfoWdt::clearLayout(QLayout *playout) {
  while (QLayoutItem *item = playout->takeAt(0)) {
    if (QWidget *widget = item->widget()) widget->deleteLater();

    if (QLayout *childLayout = item->layout()) clearLayout(childLayout);

    if (QSpacerItem *spaerItem = item->spacerItem())
      playout->removeItem(spaerItem);

    delete item;
  }

  playout->deleteLater();
  update();
}

void mPowerInfoWdt::clearWidgets(QLayout *layout) {
  if (nullptr == layout) {
    return;
  }

  //清空horizontalLayout布局内的所有元素
  QLayoutItem *child;
  while ((child = layout->takeAt(0)) != nullptr) {
    /// setParent为NULL，防止删除之后界面不消失
    if (child->widget()) {
      child->widget()->setParent(nullptr);
    } else if (child->layout()) {
      clearWidgets(child->layout());
    }
    delete child;
  }
}

// bool mPowerInfoWdt::eventFilter(QObject *watched, QEvent *event) {
//  if (event->type() == QEvent::MouseButtonPress &&
//      watched->objectName() != tr("power_val")) {
//    auto p = dynamic_cast<QDoubleSpinBox *>(watched);
//    if (p) {
//      p->setFocus();
//      p->clearFocus();
//      this->setFocus();
//    }
//  }
//  return QObject::eventFilter(watched, event);
//}

void mPowerInfoWdt::receiveQuery(QString name, queryInfo info, int a) {
  updataparam(name, info, a);
}

void mPowerInfoWdt::releaseItems() { mValItems.clear(); }
void mPowerInfoWdt::updataparam(QString name, queryInfo info, int a) {
  if (a != 1) return;

  layoutInit(info);
}
