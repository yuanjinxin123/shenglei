#include <QApplication>
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
int main(int argc, char* argv[]) {
  //  if (QT_VERSION >= QT_VERSION_CHECK(5, 6, 0))
  //    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  //  if (QT_VERSION >= QT_VERSION_CHECK(6, 0, 0))
  //    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(
  //        Qt::HighDpiScaleFactorRoundingPolicy::Floor);

  QApplication a(argc, argv);
  int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/font/SourceHanSansCN-Regular.otf"));
  QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
  if (fontFamilies.size() > 0)
  {
      QFont font;
      font.setFamily(fontFamilies[0]);//设置全局字体
      a.setFont(font);
  }

  QFile file(":/q/b.qss");
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QString result = file.readAll();

#ifdef DXJG_SHENGXIONG
      result += "QLabel#whiteLabel { border-image:url(:/img/logotitle.png); }";
      result += "QLabel#logoLb, #labelLogoName { border-image:url(:/img/logoname.png); }";
#else
      result += "QLabel#whiteLabel { border-image:url(:/img/logotitle_t.png); }";
      result += "QLabel#logoLb, #labelLogoName { border-image:url(:/img/logoname_t.png); }";
#endif

    a.setStyleSheet(result);
  }
  // Log
  Config::getIns()->init();
  Config::getIns()->Set("log", "level", TraceLevel);
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

  mSql->init();
  QTranslator translator;
  const QString qm = QDir::currentPath() + "/shenglei_zh_CN.qm";
  translator.load(":/shenglei_zh_CN.qm");
  a.installTranslator(&translator);
  //  const QStringList uiLanguages = QLocale::system().uiLanguages();
  //  for (const QString &locale : uiLanguages) {
  //    const QString baseName = "shenglei_" + QLocale(locale).name();
  //    if (translator.load(":/i18n/" + baseName)) {
  //      a.installTranslator(&translator);
  //      break;
  //    }
  //  }

  GraphicsView /*MainWindow*/ w;
  w.show();
  return a.exec();
}
