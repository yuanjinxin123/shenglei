#include "dlg_act.h"

#include <string.h>

#include <QMessageBox>>

#include "ui_dlg_act.h"
dlg_act::dlg_act(QWidget *parent) : QDialog(parent), ui(new Ui::dlg_act) {
  ui->setupUi(this);
  setWindowTitle(tr("act equipment"));
  connect(mportMg, &mportManager::sendCmd, this, &dlg_act::receive_pin_status);
}

dlg_act::~dlg_act() { delete ui; }

void dlg_act::on_btn_pin1_clicked() {
  std::string pin1 = ui->edtPin1->text().toStdString();
  if (pin1.length() != 6) {
    QMessageBox::critical(nullptr, tr("error"), tr("pin1 length error"));
    return;
  }
  sender()->blockSignals(true);
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  uint8_t *p = (uint8_t *)pin1.c_str();
  for (int i = 0; i < pin1.length(); i++) packet << p[i];
  packet << (uint8_t)0;
  mportMg->send(TIME1_PIN, val);
  sender()->blockSignals(false);
}

void dlg_act::on_pin_btn2_clicked() {
  std::string pin2 = ui->edtpin2->text().toStdString();
  if (pin2.length() != 6) {
    QMessageBox::critical(nullptr, tr("error"), tr("pin2 length error"));
    return;
  }
  sender()->blockSignals(true);
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  uint8_t *p = (uint8_t *)pin2.c_str();
  for (int i = 0; i < pin2.length(); i++) packet << p[i];
  packet << (uint8_t)0;
  mportMg->send(TIME2_PIN, val);
  sender()->blockSignals(false);
}

void dlg_act::on_pin_btn3_clicked() {
  std::string pin3 = ui->edtpin3->text().toStdString();
  if (pin3.length() != 6) {
    QMessageBox::critical(nullptr, tr("error"), tr("pin3 length error"));
    return;
  }
  sender()->blockSignals(true);
  QByteArray val;
  QDataStream packet(&val, QIODevice::ReadWrite);
  packet.setByteOrder(QDataStream::BigEndian);
  uint8_t *p = (uint8_t *)pin3.c_str();
  for (int i = 0; i < pin3.length(); i++) packet << p[i];
  packet << (uint8_t)0;
  mportMg->send(TIME3_PIN, val);
  sender()->blockSignals(false);
}

void dlg_act::on_close_btn_clicked() { reject(); }

void dlg_act::receive_pin_status(QString name, cmdData data) {
  if (data.cmd != TIME1_PIN && data.cmd != TIME2_PIN && data.cmd != TIME3_PIN) {
    return;
  }

  int n;
  if (data.cmd == TIME1_PIN) {
    n = 1;
  } else if (data.cmd == TIME2_PIN) {
    n = 2;
  } else {
    n = 3;
  }
  if (data.data.length() != 7) {
    QMessageBox::critical(nullptr, "err",
                          QString(tr("pin%1 result length error")).arg(n));
    return;
  }
  if (data.data.at(6) == 0) {
    QMessageBox::critical(nullptr, "err", QString(tr("pin%1  error")).arg(n));
  } else if (data.data.at(6) == 1) {
    QMessageBox::information(nullptr, "info",
                             QString(tr("pin%1  is ok")).arg(n));
  } else {
    QMessageBox::information(nullptr, "info",
                             QString(tr("pin%1  was used")).arg(n));
  }
}
