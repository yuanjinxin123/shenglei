#pragma once

#include <QBuffer>
#include <QByteArray>

#include "../../zlib/include/JlCompress.h"
#include "../../zlib/include/quazip.h"

class JlCompressEx : public JlCompress {
 public:
  // 文件压缩到内存buffer，zipIoDevice可以使用QBuffer zipBuffer;
  static bool CompressToBuffer(QString file, const char* password,
                               QIODevice& zipIoDevice);
  // 内存数据压缩到内存buffer
  static bool CompressToBuffer(QByteArray& sourceData, QString fileName,
                               const char* password, QIODevice& zipIoDevice);
  // 文件压缩到内存buffer
  static bool CompressToBuffer(QStringList files, const char* password,
                               QIODevice& zipIoDevice);
  /**
    \ 重写JlCompress::compressFile,添加密码参数
    \param zip Opened zip to compress the file to.
    \param fileName The full path to the source file.
    \param fileDest The full name of the file inside the archive.
    \return true if success, false otherwise.
  */
  static bool compressFileEx(QuaZip* zip, QString fileName, QString fileDest,
                             const char* password);

 public:
  // 压缩数据
  static bool CompressBuffer(QuaZip& zip, QByteArray& sourceData,
                             QString fileName, const char* password);

 public:
  // 解压，解压出来所有文件数据保存在filesList<文件名, 文件内容>
  static bool extractToBuffer(const QString& strZipFile, const char* password,
                              QList<QPair<QString, QByteArray>>& filesList);
};
