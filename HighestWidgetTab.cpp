
#include "HighestWidgetTab.h"
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QTabBar>

HighestWidgetTab::HighestWidgetTab(QWidget *parent)
  : QWidget{parent} {
  QHBoxLayout *hlayout = new QHBoxLayout(this);
  hlayout->setContentsMargins(40, 5, 0, 0);
  hlayout->setSpacing(15);

  m_btnSeedParam = new QPushButton(tr("Seed"));
  m_btnLimParam = new QPushButton(tr("Extremum"));
  m_btnAcousto = new QPushButton(tr("Regulator"));
  m_btnOther = new QPushButton(tr("Other"));

  m_btnSeedParam->setObjectName("tabBtnSeedParam");
  m_btnLimParam->setObjectName("tabBtnLimParam");
  m_btnAcousto->setObjectName("tabBtnAcousto");
  m_btnOther->setObjectName("tabBtnOther");

  hlayout->addWidget(m_btnSeedParam);
  hlayout->addWidget(m_btnLimParam);
  hlayout->addWidget(m_btnAcousto);
  hlayout->addWidget(m_btnOther);
  hlayout->addStretch();

  m_btnSeedParam->setFixedSize(110, 50);
  m_btnLimParam->setFixedSize(110, 50);
  m_btnAcousto->setFixedSize(110, 50);
  m_btnOther->setFixedSize(110, 50);

  m_mapButton[tr("Seed")] = m_btnSeedParam;
  m_mapButton[tr("Extremum")] = m_btnLimParam;
  m_mapButton[tr("Regulator")] = m_btnAcousto;
  m_mapButton[tr("Other")] = m_btnOther;

  m_mapName[m_btnSeedParam] = tr("Seed");
  m_mapName[m_btnLimParam] = tr("Extremum");
  m_mapName[m_btnAcousto]  = tr("Regulator");
  m_mapName[m_btnOther]    = tr("Other");

  QButtonGroup *btnGroup = new QButtonGroup();
  btnGroup->addButton(m_btnSeedParam);
  btnGroup->addButton(m_btnLimParam);
  btnGroup->addButton(m_btnAcousto);
  btnGroup->addButton(m_btnOther);
  connect(btnGroup, SIGNAL(buttonClicked(QAbstractButton *)),
          this, SLOT(slotButtonClicked(QAbstractButton *)));
}

void HighestWidgetTab::setWidget(QTabWidget *tabWidget) {
  m_tabWidget = tabWidget;
  m_tabWidget->tabBar()->hide();
  slotButtonClicked(m_btnSeedParam);
}

void HighestWidgetTab::setButtonImage(QWidget *widget) {
  QString style_select = "color:rgb(255,255,255); "
                         "font:14pt;"
                         "min-width: 140;"
                         "border-image: url(:/img/button_select.png);";
  QString style_normal = "color:rgb(98,98,98); "
                         "font:14pt;"
                         "min-width: 140;"
                         "border-image: url(:/img/button_normal.png); ";
  if (m_btnSeedParam == widget)
    m_btnSeedParam->setStyleSheet(style_select);
  else
    m_btnSeedParam->setStyleSheet(style_normal);

  if (m_btnLimParam == widget)
    m_btnLimParam->setStyleSheet(style_select);
  else
    m_btnLimParam->setStyleSheet(style_normal);

  if (m_btnAcousto == widget)
    m_btnAcousto->setStyleSheet(style_select);
  else
    m_btnAcousto->setStyleSheet(style_normal);

  if (m_btnOther == widget)
    m_btnOther->setStyleSheet(style_select);
  else
    m_btnOther->setStyleSheet(style_normal);
}


void HighestWidgetTab::slotButtonClicked(QAbstractButton *button) {
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
