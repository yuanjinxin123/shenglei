#include "portcfg.h"

#include <qjsondocument.h>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QMessageBox>
#include <QSerialPortInfo>

#include "log.h"
#include "mportmanager.h"
#include "sql.h"
#include "ui_portcfg.h"
#include "define.h"
Q_GLOBAL_STATIC(portCfg, g_portCfg);
portCfg::portCfg(QWidget *parent)
  : QDialog(parent),
    ui(new Ui::portCfg),
    mRate("115200"),
    mBit("8"),
    mParity(tr("none")),
    mStopbit("1") {
  ui->setupUi(this);
  Qt::WindowFlags windowFlag = Qt::Dialog;

  windowFlag |= Qt::WindowCloseButtonHint;
  setWindowFlags(windowFlag);
#ifdef SHENGXIONG
  setWindowIcon(QIcon(":/img/logo.png"));
#elif defined(SHENGLEI)
  setWindowIcon(QIcon(":/img/logo_t.png"));
#else

#endif
  init();
  serialCfg();
}

portCfg::~portCfg() {
  // ui->cboxPortName->deleteLater();

  delete ui;
}

bool portCfg::setOpenCom(mSerial *serial) {
  m_serial = serial;
  return true;
}

bool portCfg::getOpenCom(mSerial *serial) { return true; }

void portCfg::getComParam(const QString &name, com_param &param) {}
void portCfg::getComParam(QVector<com_param> &param) {
  com_param p;
  serialCfg();
  for (int i = 0; i < ui->cboxPortName->count(); i++) {
    QString itemName = ui->cboxPortName->itemText(i);
    p = {itemName, ui->cboxBaudRate->itemData(7).toInt(),
         ui->cboxDataBit->itemData(3).toInt(),
         ui->cboxParity->itemData(0).toInt(),
         ui->cboxStopBit->itemData(0).toInt()
        };
    QVariant v;
    QString decs;
    if (mSql->getValue("port", itemName, v, decs) == false) {
      continue;
    }
    if (v.isNull()) {
      // on_mOk_clicked();
      param << p;
      continue;
    }

    QJsonDocument doc;
    QJsonParseError jsonError;

    doc = QJsonDocument::fromJson(v.toByteArray(),
                                  &jsonError);  // 转化为 JSON 文档
    if (doc.isNull() || (jsonError.error != QJsonParseError::NoError)) {
      param << p;
      continue;
    }
    auto params = doc.object();
    QString key = itemName + "_num";
    if (params.contains(key) == false) {
      param << p;
      continue;
    }
    auto arr = params[key].toArray();

    p.rate = arr[0].toInt();
    p.bits = arr[1].toInt();
    p.parity = arr[2].toInt();
    p.stopBit = arr[3].toInt();

    param << p;
  }
}

bool portCfg::serialCfg() {
  QStringList m_serialPortName;
  m_serial = (mSerial *)mportMg->getSerial("com");
  if (m_serial->isOpen()) {
    m_serial->clear();
    m_serial->close();
  }

  if (QSerialPortInfo::availablePorts().empty()) {
    QMessageBox::critical(NULL, tr("error"), tr("Please insert com"),
                          QMessageBox::Yes);
    return false;
  }

  foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
    if (info.isBusy() || info.isNull()) continue;

    QSerialPort p(info.portName());
    if (p.isOpen() == false) {
      m_serialPortName << info.portName();
    }
  }

  ui->cboxPortName->clear();
  ui->cboxPortName->addItems(m_serialPortName);

  if (m_serialPortName.empty() == false) ui->cboxPortName->setCurrentIndex(0);

  return true;
}

void portCfg::init() {
  QString val;

  // getParam("com", val);

  ui->cboxBaudRate->addItem("1200", QSerialPort::Baud1200);
  ui->cboxBaudRate->addItem("2400", QSerialPort::Baud2400);
  ui->cboxBaudRate->addItem("4800", QSerialPort::Baud4800);
  ui->cboxBaudRate->addItem("9600", QSerialPort::Baud9600);
  ui->cboxBaudRate->addItem("19200", QSerialPort::Baud19200);
  ui->cboxBaudRate->addItem("38400", QSerialPort::Baud38400);
  ui->cboxBaudRate->addItem("57600", QSerialPort::Baud57600);
  ui->cboxBaudRate->addItem("115200", QSerialPort::Baud115200);

  // ui->cboxBaudRate->setCurrentText(mRate);

  ui->cboxDataBit->addItem("5", QSerialPort::Data5);
  ui->cboxDataBit->addItem("6", QSerialPort::Data6);
  ui->cboxDataBit->addItem("7", QSerialPort::Data7);
  ui->cboxDataBit->addItem("8", QSerialPort::Data8);
  // ui->cboxDataBit->setCurrentText(mBit);

  ui->cboxParity->addItem(tr("none"), QSerialPort::NoParity);

  ui->cboxParity->addItem(tr("q"), QSerialPort::OddParity);
  ui->cboxParity->addItem(tr("o"), QSerialPort::EvenParity);

  ui->cboxParity->addItem(tr("b"), QSerialPort::SpaceParity);

  ui->cboxParity->addItem(tr("m"), QSerialPort::MarkParity);

  // ui->cboxParity->setCurrentText(mParity);

  ui->cboxStopBit->addItem("1", QSerialPort::OneStop);
  ui->cboxStopBit->addItem("1.5", QSerialPort::OneAndHalfStop);
  ui->cboxStopBit->addItem("2", QSerialPort::TwoStop);

  // ui->cboxStopBit->setCurrentText(mStopbit);
}

bool portCfg::getParam(const QString &k, QString &val) {
  QVariant v;
  QString decs;
  if (mSql->getValue("port", k, v, decs) == false) {
    return false;
  }
  if (v.isNull()) {
    resetDefaultParam();
    on_mOk_clicked();
    return false;
  }
  val = v.toString();
  return parseJson(k, v.toString());
}

bool portCfg::parseJson(const QString &name, const QString &json) {
  if (name.isEmpty() || json.isEmpty()) return false;
  QJsonDocument doc;
  QJsonParseError jsonError;
  QLOG_DEBUG() << "param json " << name << " " << json;
  doc = QJsonDocument::fromJson(json.toUtf8(), &jsonError);  // 转化为 JSON 文档
  if (doc.isNull() || (jsonError.error != QJsonParseError::NoError)) {
    return false;
  }
  auto params = doc.object();
  if (params.contains(name) == false) return false;
  auto arr = params[name].toArray();
  if (arr.size() != 4) return false;
  mRate = arr[0].toString();
  mBit = arr[1].toString();
  mParity = arr[2].toString();
  mStopbit = arr[3].toString();

  return true;
}

bool portCfg::parseJson(const QString &name, const QString &json,
                        QVector<QVariant> &data) {
  if (name.isEmpty() || json.isEmpty()) return false;
  QJsonDocument doc;
  QJsonParseError jsonError;
  QLOG_DEBUG() << "param json " << name << " " << json;
  doc = QJsonDocument::fromJson(json.toUtf8(), &jsonError);  // 转化为 JSON 文档
  if (doc.isNull() || (jsonError.error != QJsonParseError::NoError)) {
    return false;
  }
  auto params = doc.object();
  if (params.contains(name) == false) return false;
  auto arr = params[name].toArray();
  if (arr.size() != 4) return false;
  for (auto it : arr) {
    data.push_back(it.toVariant());
  }
  return true;
}

QString portCfg::makeComJson(const QString &name) {
  QJsonDocument doc;
  QJsonArray array, array_num;
  QJsonObject json;
  QJsonArray port;
  if (name.isEmpty()) return QString();
  array.append(ui->cboxBaudRate->currentText());
  array.append(ui->cboxDataBit->currentText());
  array.append(ui->cboxParity->currentText());
  array.append(ui->cboxStopBit->currentText());

  json.insert(name.toUtf8(), QJsonValue(array));

  array_num.append(ui->cboxBaudRate->currentData().toInt());
  array_num.append(ui->cboxDataBit->currentData().toInt());
  array_num.append(ui->cboxParity->currentData().toInt());
  array_num.append(ui->cboxStopBit->currentData().toInt());
  json.insert(name.toUtf8() + "_num", QJsonValue(array_num));
  doc.setObject(json);
  return doc.toJson();
}

void portCfg::resetDefaultParam() {
  mRate = "115200";
  mBit = "8";
  mParity = tr("none");
  mStopbit = "1";
  ui->cboxBaudRate->setCurrentText(mRate);
  ui->cboxDataBit->setCurrentText(mBit);
  ui->cboxParity->setCurrentText(mParity);
  ui->cboxStopBit->setCurrentText(mStopbit);
}

portCfg *portCfg::ins() { return g_portCfg; }

void portCfg::on_mCacel_clicked() { reject(); }

void portCfg::on_cboxPortName_currentIndexChanged(const QString &name) {
  QString val;
  getParam(name, val);
  ui->cboxBaudRate->setCurrentText(mRate);
  ui->cboxDataBit->setCurrentText(mBit);
  ui->cboxParity->setCurrentText(mParity);
  ui->cboxStopBit->setCurrentText(mStopbit);
  mSerial s;
  s.setParam(name,
             (QSerialPort::BaudRate)ui->cboxBaudRate->currentData().toInt(),
             (QSerialPort::StopBits)ui->cboxStopBit->currentData().toInt(),
             (QSerialPort::DataBits)ui->cboxDataBit->currentData().toInt(),
             (QSerialPort::Parity)ui->cboxParity->currentData().toInt());
}

void portCfg::on_mOk_clicked() {
  auto json = makeComJson(ui->cboxPortName->currentText());
  QLOG_DEBUG() << "Save json :" << json;
  if (mSql->setKey("port", ui->cboxPortName->currentText(), json,
                   "com1 param") == false) {
    QLOG_ERROR() << "Save " << ui->cboxPortName->currentText() << " is err";
  }
  accept();
}
