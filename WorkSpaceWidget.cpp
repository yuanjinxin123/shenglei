
#include "WorkSpaceWidget.h"
#include "config.h"
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

  m_mapButton[tr("home")] = m_btnHome;
  m_mapButton[tr("electric")] = m_btnParam;
  m_mapButton[tr("semp")] = m_btnDoublefreq;
  m_mapButton[tr("acousto")] = m_btnControl;
  m_mapButton[tr("warn limt")] = m_btnState;
  m_mapButton[tr("highest")] = m_btnSenior;

  m_mapLabel[tr("home")] = m_lableHome;
  m_mapLabel[tr("electric")] = m_lableParam;
  m_mapLabel[tr("semp")] = m_lableDoublefreq;
  m_mapLabel[tr("acousto")] = m_lableControl;
  m_mapLabel[tr("warn limt")] = m_lableState;

  m_mapName[m_btnHome] = tr("home");
  m_mapName[m_btnParam] = tr("electric");
  m_mapName[m_btnDoublefreq] = tr("semp");
  m_mapName[m_btnControl] = tr("acousto");
  m_mapName[m_btnState] = tr("warn limt");
  m_mapName[m_btnSenior] = tr("highest");

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
    qDebug() << tr("home") << "==" << tr("warn limt") << "==" << tr("highest");
    if (label == tr("home"))
      setButtonImage(button);
    button->setVisible(true);
  }

  if (m_mapLabel.contains(label)) {
    if (label == tr("warn limt") && !m_btnSenior->isVisible())
      m_mapLabel.value(label)->setVisible(false);
    else
      m_mapLabel.value(label)->setVisible(true);
  }

  if (!m_mapLabel.contains(label)) {
    m_mapLabel.value(tr("warn limt"))->setVisible(true);
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
    m_mapLabel.value(tr("warn limt"))->setVisible(false);
  }

  m_tabWidget->removeTab(index);

  m_tabWidget->setCurrentIndex(0);
  QPushButton *button = m_mapButton.value(tr("home"));
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
  QString langFlag = "";
  if (Config::getIns()->Get("main/lang").toString() != "zh") {
    langFlag = "_en";
  }
  if (m_btnHome == widget)
    m_btnHome->setStyleSheet(QString("QPushButton#tabBtnHome{ border-image: url(:/img/workspace_tab_home_h%1.png); }").arg(langFlag));
  else
    m_btnHome->setStyleSheet(QString("QPushButton#tabBtnHome{ border-image: url(:/img/workspace_tab_home%1.png); }").arg(langFlag));

  if (m_btnParam == widget)
    m_btnParam->setStyleSheet(QString("QPushButton#tabBtnParam{ border-image: url(:/img/workspace_tab_param_h%1.png); }").arg(langFlag));
  else
    m_btnParam->setStyleSheet(QString("QPushButton#tabBtnParam{ border-image: url(:/img/workspace_tab_param%1.png); }").arg(langFlag));

  if (m_btnDoublefreq == widget)
    m_btnDoublefreq->setStyleSheet(QString("QPushButton#tabBtnDoublefreq{ border-image: url(:/img/workspace_tab_doublefreq_h%1.png); }").arg(langFlag));
  else
    m_btnDoublefreq->setStyleSheet(QString("QPushButton#tabBtnDoublefreq{ border-image: url(:/img/workspace_tab_doublefreq%1.png); }").arg(langFlag));

  if (m_btnControl == widget)
    m_btnControl->setStyleSheet(QString("QPushButton#tabBtnControl{ border-image: url(:/img/workspace_tab_cotrol_h%1.png); }").arg(langFlag));
  else
    m_btnControl->setStyleSheet(QString("QPushButton#tabBtnControl{ border-image: url(:/img/workspace_tab_cotrol%1.png); }").arg(langFlag));

  if (m_btnState == widget)
    m_btnState->setStyleSheet(QString("QPushButton#tabBtnState{ border-image: url(:/img/workspace_tab_state_h%1.png); }").arg(langFlag));
  else
    m_btnState->setStyleSheet(QString("QPushButton#tabBtnState{ border-image: url(:/img/workspace_tab_state%1.png); }").arg(langFlag));

  if (m_btnSenior == widget)
    m_btnSenior->setStyleSheet(QString("QPushButton#tabBtnSenior{ border-image: url(:/img/workspace_tab_senior_h%1.png); }").arg(langFlag));
  else
    m_btnSenior->setStyleSheet(QString("QPushButton#tabBtnSenior{ border-image: url(:/img/workspace_tab_senior%1.png); }").arg(langFlag));
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
