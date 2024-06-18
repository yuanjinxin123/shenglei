
#ifndef MACOUSTOWIDGETTAB_H
#define MACOUSTOWIDGETTAB_H

#include <QWidget>
#include <QPushButton>
#include <QMap>
#include <QTabWidget>

class MacoustoWidgetTab: public QWidget
{
    Q_OBJECT
public:
    explicit MacoustoWidgetTab(QWidget *parent = nullptr);

    void setWidget(QTabWidget *tabWidget);

    void setButtonImage(QWidget *widget);

public slots:
    void slotButtonClicked(QAbstractButton *button);

signals:

private:
    QPushButton *m_btnBaseParam;
    QPushButton *m_btnPowerCor;
    QPushButton *m_btnPluse;

    QTabWidget *m_tabWidget;
    QMap<QString, QPushButton*> m_mapButton;
    QMap<QAbstractButton*, QString> m_mapName;
};

#endif // MACOUSTOWIDGETTAB_H
