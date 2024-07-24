#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QDir>
#include <QLocale>
#include <QTranslator>
#include "GraphicsView.h"

#include "config.h"
#include "log.h"
#include "mainwindow.h"
#include "sql.h"
#include <QFontDatabase>
#include <QStandardPaths>
#include "define.h"
#ifdef _WIN32
#include <Windows.h>
#endif


qreal getscaleFactor() {
  qreal dpi = 96.0; // 默认 DPI
#ifdef _WIN32
  HDC screen = GetDC(0);
  int dpiX = GetDeviceCaps(screen, LOGPIXELSX);
  ReleaseDC(0, screen);
  dpi = static_cast<qreal>(dpiX);
#endif
  qreal scaleFactor = dpi / 96.0;
  return scaleFactor;
}

int main(int argc, char *argv[]) {

  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  qreal scaleFactor = getscaleFactor();
  if (scaleFactor == 1.25 || scaleFactor == 1.75)
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);

  QApplication a(argc, argv);
  a.setQuitOnLastWindowClosed(false);
  int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/SourceHanSansCN-Regular.otf"));
  QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
  if (fontFamilies.size() > 0) {
    QFont font;
    font.setFamily(fontFamilies[0]);//设置全局字体
    a.setFont(font);
  }

  QFile file(":/q/b.qss");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QString result = file.readAll();

#ifdef SHENGXIONG
    a.setWindowIcon(QIcon(":/img/logo.png"));
    result += "QLabel#whiteLabel { border-image:url(:/img/logotitle.png); }";
    result += "QLabel#logoLb, #labelLogoName { border-image:url(:/img/logoname.png); }";
#elif defined(SHENGLEI)
    a.setWindowIcon(QIcon(":/img/logo_t.png"));
    result += "QLabel#whiteLabel { border-image:url(:/img/logotitle_t.png); }";
    result += "QLabel#logoLb, #labelLogoName { border-image:url(:/img/logoname_t.png); }";
#else
    result += "QLabel#whiteLabel { border-image:url(:/img/logotitle_no.png); }";
#endif

    a.setStyleSheet(result);
  }
  // setting
  QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  QDir().mkpath(appDataPath);
  QString ConfFilePath = appDataPath + "/Config.ini";
  if (!QFile::exists(ConfFilePath)) {
    QFile::copy(QCoreApplication::applicationDirPath() + "/Config.ini", ConfFilePath);
  }
  Config::getIns()->init(ConfFilePath);
  //log
  auto level = Config::getIns()->Get("log", "level", TraceLevel);

  log::getInstance()->setLoggingLevel((Level)level.toUInt());
  log::getInstance()->setDestIsCmd();
  QString logpath = QDir::currentPath() + "/log";
  QDir dir(logpath);
  if (!dir.exists()) {
    dir.mkpath(logpath);  //创建多级目录
  }

  logpath += "/log.txt";
  log::getInstance()->setDestIsFile(logpath, 1024 * 1024, 10);
  QTranslator translator;
  mSql->init();
  if (Config::getIns()->Get("main/lang").toString() == "zh") {
    translator.load(":/shenglei_zh_CN.qm");
    a.installTranslator(&translator);
  }

  //  const QStringList uiLanguages = QLocale::system().uiLanguages();
  //  for (const QString &locale : uiLanguages) {
  //    const QString baseName = "shenglei_" + QLocale(locale).name();
  //    if (translator.load(":/i18n/" + baseName)) {
  //      a.installTranslator(&translator);
  //      break;
  //    }
  //  }


  GraphicsView::GetInstancePointer()->show();
  return a.exec();
}
