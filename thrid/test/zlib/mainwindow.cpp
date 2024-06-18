#include "mainwindow.h"

#include <QByteArray>
#include <QString>

#include "jlcompressex.h"
#include "ui_mainwindow.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  // JlCompressEx compress;
  QByteArray test = "test";
  QString filename = "test.txt";
  // QuaZip zip("test11.zip");
  // compress.CompressToBuffer(zip,)
  QFile file2("323.zip");
  JlCompressEx::CompressToBuffer(test, filename, "123456", file2);
}

MainWindow::~MainWindow() { delete ui; }
