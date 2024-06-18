#ifndef MACOUSTOBASE_H
#define MACOUSTOBASE_H

#include <QWidget>

namespace Ui {
class mAcoustoBase;
}

class mAcoustoBase : public QWidget {
  Q_OBJECT

 public:
  explicit mAcoustoBase(QWidget *parent = nullptr);
  ~mAcoustoBase();
 protected slots:
  void changePsoOrPod(uint8_t);

 protected:
  void init();

 private:
  Ui::mAcoustoBase *ui;
};

#endif  // MACOUSTOBASE_H
