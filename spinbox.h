#ifndef SPINBOX_H
#define SPINBOX_H

#include <QDoubleSpinBox>
#include <QObject>
#include <QSpinBox>
class spinbox : public QSpinBox {
  Q_OBJECT
 public:
  explicit spinbox(QWidget* parent = nullptr);
  void RegFilterObj(QObject* obj);
  virtual ~spinbox();
  virtual bool getHasFouce();

 protected:
  // bool eventFilter(QObject* obj, QEvent* event);
  void enterEvent(QEvent* e);  //鼠标进入事件
  void leaveEvent(QEvent* e);

 protected:
  bool mIsHasFouce = false;
};
class doubleSpinbox : public QDoubleSpinBox {
  Q_OBJECT
 public:
  explicit doubleSpinbox(QWidget* parent = nullptr);
  void RegFilterObj(QObject* obj);
  virtual ~doubleSpinbox();
  virtual bool getHasFouce();

 protected:
  // bool eventFilter(QObject* obj, QEvent* event);
  void enterEvent(QEvent* e);  //鼠标进入事件
  void leaveEvent(QEvent* e);

 protected:
  bool mIsHasFouce = false;
};
#endif  // SPINBOX_H
