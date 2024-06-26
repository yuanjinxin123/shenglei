
#include "MacoustoWidgetTab.h"

#include <QHBoxLayout>
#include <QButtonGroup>
#include <QTabBar>

MacoustoWidgetTab::MacoustoWidgetTab(QWidget *parent)
  : QWidget{parent} {
  QHBoxLayout *hlayout = new QHBoxLayout(this);
  hlayout->setContentsMargins(45, 10, 0, 0);
  hlayout->setSpacing(15);

  m_btnBaseParam = new QPushButton(tr("Basic Parameters"));
  m_btnPowerCor = new QPushButton(tr("Power Parameters"));
  m_btnPluse = new QPushButton(tr("Pulse Editing"));

  m_btnBaseParam->setObjectName("tabBtnBaseParam");
  m_btnPowerCor->setObjectName("tabBtnPowerCor");
  m_btnPluse->setObjectName("tabBtnPluseEdit");

  hlayout->addStretch();
  hlayout->addWidget(m_btnBaseParam);
  hlayout->addWidget(m_btnPowerCor);
  hlayout->addWidget(m_btnPluse);
  hlayout->addStretch();

  m_btnBaseParam->setFixedSize(120, 45);
  m_btnPowerCor->setFixedSize(120, 45);
  m_btnPluse->setFixedSize(120, 45);

  m_mapButton["基础参数"] = m_btnBaseParam;
  m_mapButton["功率校正"] = m_btnPowerCor;
  m_mapButton["脉冲编辑"] = m_btnPluse;

  m_mapName[m_btnBaseParam] = "基础参数";
  m_mapName[m_btnPowerCor] = "功率校正";
  m_mapName[m_btnPluse]  = "脉冲编辑";

  QButtonGroup *btnGroup = new QButtonGroup();
  btnGroup->addButton(m_btnBaseParam);
  btnGroup->addButton(m_btnPowerCor);
  btnGroup->addButton(m_btnPluse);
  connect(btnGroup, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(slotButtonClicked(QAbstractButton *)));
}

void MacoustoWidgetTab::setWidget(QTabWidget *tabWidget) {
  m_tabWidget = tabWidget;
  m_tabWidget->tabBar()->hide();
  slotButtonClicked(m_btnBaseParam);
}

void MacoustoWidgetTab::setButtonImage(QWidget *widget) {
  QString style_select = "color:rgb(255,255,255);"
                         "font:14pt;"
                         "border-image: url(:/img/button_select.png);";
  QString style_normal = "color:rgb(98,98,98); "
                         "font:14pt;"
                         "border-image: url(:/img/button_normal.png); ";
  if (m_btnBaseParam == widget)
    m_btnBaseParam->setStyleSheet(style_select);
  else
    m_btnBaseParam->setStyleSheet(style_normal);

  if (m_btnPowerCor == widget)
    m_btnPowerCor->setStyleSheet(style_select);
  else
    m_btnPowerCor->setStyleSheet(style_normal);

  if (m_btnPluse == widget)
    m_btnPluse->setStyleSheet(style_select);
  else
    m_btnPluse->setStyleSheet(style_normal);
}

void MacoustoWidgetTab::slotButtonClicked(QAbstractButton *button) {
  setButtonImage(button);

  if (m_mapName.contains(button)) {
    QString name = m_mapName.value(button);
    for (int i = 0; i < m_tabWidget->count(); i++) {
      if (m_tabWidget->tabText(i) == name) {
        m_tabWidget->setCurrentIndex(i);
        break;
      }
    }
  }
}
