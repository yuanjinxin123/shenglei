#ifndef MACOUSTOPARAM_H
#define MACOUSTOPARAM_H

#include <QWidget>

#include "mwidget.h"
#include "xiaohao.h"
namespace Ui {
class macoustoParam;
}

class macoustoParam : public mWidget {
  Q_OBJECT

 public:
  explicit macoustoParam(QWidget *parent = nullptr);
  ~macoustoParam();
  void init();
 protected slots:
  void changVer(uint8_t index);

 private:
  Ui::macoustoParam *ui;
  xiaohao *xh;
};

#endif  // MACOUSTOPARAM_H
