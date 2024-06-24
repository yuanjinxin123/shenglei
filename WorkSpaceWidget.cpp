
#include "WorkSpaceWidget.h"

#include <QTabBar>


WorkSpaceWidget::WorkSpaceWidget(QWidget *parent)
  : QWidget(parent)
  , m_topWidget(nullptr)
  , m_tabWidget(nullptr) {
}

void WorkSpaceWidget::setWidget(QWidget *topWidget, QWidget *leftWidget, QTabWidget *tabWidget, QWidget *highestWidget) {
  m_topWidget = topWidget;
  m_leftWidget = leftWidget;
  m_tabWidget = tabWidget;
  m_highestWidget = highestWidget;
  m_highestWidget->setVisible(false);
  m_tabWidget->tabBar()->hide();

  QHBoxLayout *hlayout = new QHBoxLayout(topWidget);
  hlayout->setContentsMargins(42, 2, 2, 2);
  hlayout->setSpacing(0);
  topWidget->setLayout(hlayout);

  m_btnHome = new QPushButton();
  m_lableHome = new QLabel();
  m_btnParam = new QPushButton();
  m_lableParam = new QLabel();
  m_btnDoublefreq = new QPushButton();
  m_lableDoublefreq = new QLabel();
  m_btnControl = new QPushButton();
  m_lableControl = new QLabel();
  m_btnState = new QPushButton();
  m_lableState = new QLabel();
  m_btnSenior = new QPushButton();

  m_btnHome->setObjectName("tabBtnHome");
  m_lableHome->setObjectName("tabLabelHome");
  m_btnParam->setObjectName("tabBtnParam");
  m_lableParam->setObjectName("tabLabelParam");
  m_btnDoublefreq->setObjectName("tabBtnDoublefreq");
  m_lableDoublefreq->setObjectName("tabLabelDoublefreq");
  m_btnControl->setObjectName("tabBtnControl");
  m_lableControl->setObjectName("tabLabelControl");
  m_btnState->setObjectName("tabBtnState");
  m_lableState->setObjectName("tabLabelState");
  m_btnSenior->setObjectName("tabBtnSenior");

  hlayout->addWidget(m_btnHome);
  hlayout->addWidget(m_lableHome);
  hlayout->addWidget(m_btnParam);
  hlayout->addWidget(m_lableParam);
  hlayout->addWidget(m_btnControl);
  hlayout->addWidget(m_lableControl);
  hlayout->addWidget(m_btnDoublefreq);
  hlayout->addWidget(m_lableDoublefreq);
  hlayout->addWidget(m_btnState);
  hlayout->addWidget(m_lableState);
  hlayout->addWidget(m_btnSenior);
  hlayout->addStretch();

  m_lableHome->setFixedSize(30, 80);
  m_lableParam->setFixedSize(30, 80);
  m_lableDoublefreq->setFixedSize(30, 80);
  m_lableControl->setFixedSize(30, 80);
  m_lableState->setFixedSize(30, 80);

  m_btnHome->setFixedSize(180, 95);
  m_btnParam->setFixedSize(180, 95);
  m_btnDoublefreq->setFixedSize(180, 95);
  m_btnControl->setFixedSize(180, 95);
  m_btnState->setFixedSize(180, 95);
  m_btnSenior->setFixedSize(180, 95);

//     首页 首页
//     电流 参数
//     温度 倍频
//     声光 控制
//     系统 状态监测
//     高级 高级

  m_mapButton["首页"] = m_btnHome;
  m_mapButton["电流"] = m_btnParam;
  m_mapButton["温度"] = m_btnDoublefreq;
  m_mapButton["声光"] = m_btnControl;
  m_mapButton["系统"] = m_btnState;
  m_mapButton["高级"] = m_btnSenior;

  m_mapLabel["首页"] = m_lableHome;
  m_mapLabel["电流"] = m_lableParam;
  m_mapLabel["温度"] = m_lableDoublefreq;
  m_mapLabel["声光"] = m_lableControl;
  m_mapLabel["系统"] = m_lableState;

  m_mapName[m_btnHome]       = "首页";
  m_mapName[m_btnParam]      = "电流";
  m_mapName[m_btnDoublefreq] = "温度";
  m_mapName[m_btnControl]    = "声光";
  m_mapName[m_btnState]      = "系统";
  m_mapName[m_btnSenior]     = "高级";

  QMap<QString, QPushButton *>::iterator iterButton = m_mapButton.begin();
  for (; iterButton != m_mapButton.end(); iterButton++) {
    iterButton.value()->setVisible(false);
  }

  QMap<QString, QLabel *>::iterator iterLabel = m_mapLabel.begin();
  for (; iterLabel != m_mapLabel.end(); iterLabel++) {
    iterLabel.value()->setVisible(false);
  }

  QButtonGroup *btnGroup = new QButtonGroup();
  btnGroup->addButton(m_btnHome);
  btnGroup->addButton(m_btnParam);
  btnGroup->addButton(m_btnDoublefreq);
  btnGroup->addButton(m_btnControl);
  btnGroup->addButton(m_btnState);
  btnGroup->addButton(m_btnSenior);
  connect(btnGroup, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(slotButtonClicked(QAbstractButton *)));
}
#include <QDebug>
int WorkSpaceWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label) {
  if (m_mapButton.contains(label)) {
    QPushButton *button = m_mapButton.value(label);
    if (label == "首页")
      setButtonImage(button);
    button->setVisible(true);
  }

  if (m_mapLabel.contains(label)) {
    if (label == "系统" && !m_btnSenior->isVisible())
      m_mapLabel.value(label)->setVisible(false);
    else
      m_mapLabel.value(label)->setVisible(true);
  }

  if (!m_mapLabel.contains(label)) {
    m_mapLabel.value("系统")->setVisible(true);
  }

  // qDebug() << label;
  return m_tabWidget->addTab(widget, icon, label);
}

int WorkSpaceWidget::insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label) {
  if (m_mapButton.contains(label)) {
    QPushButton *button = m_mapButton.value(label);
    button->setVisible(true);
  }

  if (m_mapLabel.contains(label)) {
    m_mapLabel.value(label)->setVisible(true);
  }

  return m_tabWidget->insertTab(index, widget, icon, label);
}

void WorkSpaceWidget::removeTab(int index) {
  const QString label = m_tabWidget->tabText(index);
  if (m_mapButton.contains(label)) {
    QPushButton *button = m_mapButton.value(label);
    button->setVisible(false);
  }

  if (m_mapLabel.contains(label)) {
    m_mapLabel.value(label)->setVisible(false);
  }

  if (!m_mapLabel.contains(label)) {
    m_mapLabel.value("系统")->setVisible(false);
  }

  m_tabWidget->removeTab(index);

  m_tabWidget->setCurrentIndex(0);
  QPushButton *button = m_mapButton.value("首页");
  if (button)
    setButtonImage(button);
}

QString WorkSpaceWidget::tabText(int index) const {
  return m_tabWidget->tabText(index);
}

QWidget *WorkSpaceWidget::widget(int index) const {
  return m_tabWidget->widget(index);
}

int WorkSpaceWidget::count() const {
  return m_tabWidget->count();
}

void WorkSpaceWidget::clear() {
  m_tabWidget->clear();
}

void WorkSpaceWidget::setButtonImage(QWidget *widget) {
  if (m_btnHome == widget)
    m_btnHome->setStyleSheet("QPushButton#tabBtnHome{ border-image: url(:/img/workspace_tab_home_h.png); }");
  else
    m_btnHome->setStyleSheet("QPushButton#tabBtnHome{ border-image: url(:/img/workspace_tab_home.png); }");

  if (m_btnParam == widget)
    m_btnParam->setStyleSheet("QPushButton#tabBtnParam{ border-image: url(:/img/workspace_tab_param_h.png); }");
  else
    m_btnParam->setStyleSheet("QPushButton#tabBtnParam{ border-image: url(:/img/workspace_tab_param.png); }");

  if (m_btnDoublefreq == widget)
    m_btnDoublefreq->setStyleSheet("QPushButton#tabBtnDoublefreq{ border-image: url(:/img/workspace_tab_doublefreq_h.png); }");
  else
    m_btnDoublefreq->setStyleSheet("QPushButton#tabBtnDoublefreq{ border-image: url(:/img/workspace_tab_doublefreq.png); }");

  if (m_btnControl == widget)
    m_btnControl->setStyleSheet("QPushButton#tabBtnControl{ border-image: url(:/img/workspace_tab_cotrol_h.png); }");
  else
    m_btnControl->setStyleSheet("QPushButton#tabBtnControl{ border-image: url(:/img/workspace_tab_cotrol.png); }");

  if (m_btnState == widget)
    m_btnState->setStyleSheet("QPushButton#tabBtnState{ border-image: url(:/img/workspace_tab_state_h.png); }");
  else
    m_btnState->setStyleSheet("QPushButton#tabBtnState{ border-image: url(:/img/workspace_tab_state.png); }");

  if (m_btnSenior == widget)
    m_btnSenior->setStyleSheet("QPushButton#tabBtnSenior{ border-image: url(:/img/workspace_tab_senior_h.png); }");
  else
    m_btnSenior->setStyleSheet("QPushButton#tabBtnSenior{ border-image: url(:/img/workspace_tab_senior.png); }");
}

void WorkSpaceWidget::slotButtonClicked(QAbstractButton *button) {
  setButtonImage(button);

  if (m_mapName.contains(button)) {
    QString name = m_mapName.value(button);
    if (name == "高级") {
      m_highestWidget->setVisible(true);
      if (m_leftWidget->layout() != nullptr)
        m_leftWidget->layout()->setContentsMargins(40, 28, 40, 15);
    } else {
      m_highestWidget->setVisible(false);
      if (m_leftWidget->layout() != nullptr)
        m_leftWidget->layout()->setContentsMargins(40, 45, 40, 15);
    }
    for (int i = 0; i < m_tabWidget->count(); i++) {
      if (m_tabWidget->tabText(i) == name) {
        m_tabWidget->setCurrentIndex(i);
        break;
      }
    }
  }
}
