
#ifndef HIGHESTWIDGETTAB_H
#define HIGHESTWIDGETTAB_H

#include <QWidget>
#include <QPushButton>
#include <QMap>
#include <QTabWidget>

class HighestWidgetTab : public QWidget
{
    Q_OBJECT
public:
    explicit HighestWidgetTab(QWidget *parent = nullptr);

    void setWidget(QTabWidget *tabWidget);

    void setButtonImage(QWidget *widget);

public slots:
    void slotButtonClicked(QAbstractButton *button);

signals:


private:
    QPushButton *m_btnSeedParam;
    QPushButton *m_btnLimParam;
    QPushButton *m_btnAcousto;
    QPushButton *m_btnOther;

    QTabWidget *m_tabWidget;
    QMap<QString, QPushButton*> m_mapButton;
    QMap<QAbstractButton*, QString> m_mapName;
};

#endif // HIGHESTWIDGETTAB_H
