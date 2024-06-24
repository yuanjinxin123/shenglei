#include "JlCompressEx.h"

#include <QDebug>

bool JlCompressEx::CompressToBuffer(QString file, const char *password,
                                    QIODevice &zipIoDevice) {
  QuaZip zip(&zipIoDevice);
  // QDir().mkpath(QFileInfo(fileCompressed).absolutePath());
  if (!zip.open(QuaZip::mdCreate)) {
    return false;
  }
  // Aggiungo il file
  if (!compressFileEx(&zip, file, QFileInfo(file).fileName(), password)) {
    return false;
  }

  // Chiudo il file zip
  zip.close();
  if (zip.getZipError() != UNZ_OK) {
    return false;
  }

  return true;
}

bool JlCompressEx::CompressToBuffer(QByteArray &sourceData, QString fileName,
                                    const char *password,
                                    QIODevice &zipIoDevice) {
  QuaZip zip(&zipIoDevice);
  // QDir().mkpath(QFileInfo(fileCompressed).absolutePath());
  if (!zip.open(QuaZip::mdCreate)) {
    return false;
  }

  if (!CompressBuffer(zip, sourceData, fileName, password)) {
    return false;
  }

  // Chiudo il file zip
  zip.close();
  if (zip.getZipError() != UNZ_OK) {
    return false;
  }

  return true;
}

bool JlCompressEx::CompressToBuffer(QStringList files, const char *password,
                                    QIODevice &zipIoDevice) {
  // Creo lo zip
  QuaZip zip(&zipIoDevice);
  // QDir().mkpath(QFileInfo(fileCompressed).absolutePath());
  if (!zip.open(QuaZip::mdCreate)) {
    return false;
  }

  // Comprimo i file
  QFileInfo info;
  for (int index = 0; index < files.size(); ++index) {
    const QString &file(files.at(index));
    info.setFile(file);
    if (!info.exists() ||
        !compressFileEx(&zip, file, info.fileName(), password)) {
      return false;
    }
  }

  // Chiudo il file zip
  zip.close();
  if (zip.getZipError() != 0) {
    return false;
  }

  return true;
}
static bool copyData(QIODevice &inFile, QIODevice &outFile) {
  while (!inFile.atEnd()) {
    char buf[4096];
    qint64 readLen = inFile.read(buf, 4096);
    if (readLen <= 0) return false;
    if (outFile.write(buf, readLen) != readLen) return false;
  }
  return true;
}
bool JlCompressEx::compressFileEx(QuaZip *zip, QString fileName,
                                  QString fileDest, const char *password) {
  // zip: oggetto dove aggiungere il file
  // fileName: nome del file reale
  // fileDest: nome del file all'interno del file compresso

  // Controllo l'apertura dello zip
  if (!zip) return false;
  if (zip->getMode() != QuaZip::mdCreate &&
      zip->getMode() != QuaZip::mdAppend && zip->getMode() != QuaZip::mdAdd)
    return false;

  // Apro il file originale
  QFile inFile;
  inFile.setFileName(fileName);
  if (!inFile.open(QIODevice::ReadOnly)) return false;

  // Apro il file risulato
  QuaZipFile outFile(zip);
  if (!outFile.open(QIODevice::WriteOnly,
                    QuaZipNewInfo(fileDest, inFile.fileName()), password))
    return false;

  // Copio i dati
  if (!copyData(inFile, outFile) || outFile.getZipError() != UNZ_OK) {
    return false;
  }

  // Chiudo i file
  outFile.close();
  if (outFile.getZipError() != UNZ_OK) return false;
  inFile.close();

  return true;
}

bool JlCompressEx::CompressBuffer(QuaZip &zip, QByteArray &sourceData,
                                  QString fileName, const char *password) {
  // Controllo l'apertura dello zip
  if (zip.getMode() != QuaZip::mdCreate && zip.getMode() != QuaZip::mdAppend &&
      zip.getMode() != QuaZip::mdAdd) {
    return false;
  }
  // Apro il file risulato
  QuaZipFile outFile(&zip);
  if (!outFile.open(QIODevice::WriteOnly, QuaZipNewInfo(fileName), password)) {
    return false;
  }
  // Copio i dati
  if (outFile.write(sourceData) != sourceData.size()) {
    return false;
  }
  if (outFile.getZipError() != UNZ_OK) {
    return false;
  }

  // Chiudo i file
  outFile.close();
  if (outFile.getZipError() != UNZ_OK) {
    return false;
  }

  return true;
}

bool JlCompressEx::extractToBuffer(
  const QString &strZipFile, const char *password,
  QList<QPair<QString, QByteArray>> &filesList) {
  QuaZip zip(strZipFile);
  if (!zip.open(QuaZip::mdUnzip)) {
    qWarning() << "open zip file error:" << zip.getZipError()
               << ". file:" << strZipFile;
    return false;
  }
  for (bool more = zip.goToFirstFile(); more; more = zip.goToNextFile()) {
    QuaZipFileInfo64 info;
    if (!zip.getCurrentFileInfo(&info)) {
      continue;
    }
    qDebug() << "in zip: fileName:" << info.name
             << ", compressedSize:" << info.compressedSize
             << ", uncompressedSize:" << info.uncompressedSize;
    QString strFileName = info.name;  // 如果为多层目录则包含目录路径

    if (0 == info.compressedSize ||
        0 == info.uncompressedSize) {  // 文件夹也会遍历出来，大小为0
      continue;
    }

    // 读取文件数据
    QuaZipFile zipFile(&zip);
    if (!zipFile.open(QIODevice::ReadOnly, password)) {
      qWarning() << "open file error:" << zipFile.getZipError()
                 << ". file name:" << strFileName;
      continue;
    }
    if (zipFile.getZipError() != UNZ_OK) {
      qWarning() << "open file error:" << zipFile.getZipError();
      continue;
    }
    // qDebug() << "file size:" << zipFile.size() << " isOpen:" <<
    // zipFile.isOpen(); continue;
    QByteArray fileData;
    while (!zipFile.atEnd()) {
      QByteArray dataTmp = zipFile.readAll();
      if (dataTmp.isEmpty()) {  // 当密码不正确时打开文件会成功，这里会读取失败
        zipFile.close();
        zip.close();
        return false;
      }
      if (zipFile.getZipError() != UNZ_OK) {
        zipFile.close();
        zip.close();
        return false;
      }
      fileData += dataTmp;
    }
    filesList.append(QPair<QString, QByteArray>(strFileName, fileData));

    zipFile.close();
  }
  //
  zip.close();
  if (zip.getZipError() != 0) {
  }
  return true;
}
