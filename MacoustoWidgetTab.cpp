
#include "MacoustoWidgetTab.h"

#include <QHBoxLayout>
#include <QButtonGroup>
#include <QTabBar>
#include <QDebug>
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

  m_mapButton[tr("Basic Parameters")] = m_btnBaseParam;
  m_mapButton[tr("Power Parameters")] = m_btnPowerCor;
  m_mapButton[tr("Pulse Editing")] = m_btnPluse;

  m_mapName[m_btnBaseParam] = tr("Basic Parameters");
  m_mapName[m_btnPowerCor] = tr("Power Parameters");
  m_mapName[m_btnPluse]  = tr("Pulse Editing");

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
      qDebug() << m_tabWidget->tabText(i) << "===" << name;
      if (m_tabWidget->tabText(i) == name) {
        m_tabWidget->setCurrentIndex(i);
        break;
      }
    }
  }
}
