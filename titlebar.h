#ifndef TITLE_BAR
#define TITLE_BAR

#include <QWidget>
#include <QRadioButton>

class QLabel;
class QPushButton;

class TitleBar : public QWidget {
  Q_OBJECT

 public:
  explicit TitleBar(QWidget *parent = 0);
  ~TitleBar();

  void setFrameWindow(QWidget *widget) {
    m_frameWidget = widget;
  }
  QWidget *getFrameWindow() {
    if (m_frameWidget != nullptr)
      return m_frameWidget;
    return window();
  }

 protected:

  // 双击标题栏进行界面的最大化/还原
  virtual void mouseDoubleClickEvent(QMouseEvent *event);

  // 进行鼠界面的拖动
  virtual void mousePressEvent(QMouseEvent *event);
  // 设置界面标题与图标
  virtual bool eventFilter(QObject *obj, QEvent *event);

 private slots:
  // 进行最小化、最大化/还原、关闭操作
  void onClicked();
  void onChangeChinese();
  void onChangeEnglish();
 private:
  // 最大化/还原
  void updateMaximize();

 private:
  QLabel *m_pIconLabel;
  QLabel *m_pTitleLabel;
  QLabel *m_pChinese;
  QLabel *m_pEnglish;
  QRadioButton *m_pChineseButton;
  QRadioButton *m_pEnglishButton;
  QRadioButton *mRMode1;
  QPushButton *m_pMinimizeButton;
  QPushButton *m_pMaximizeButton;
  QPushButton *m_pCloseButton;
  QWidget *m_frameWidget;
};

#endif // TITLE_BAR
