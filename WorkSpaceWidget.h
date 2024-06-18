
#ifndef WORKSPACEWIDGET_H
#define WORKSPACEWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QMap>
#include <QHBoxLayout>
#include <QLabel>

class WorkSpaceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit WorkSpaceWidget(QWidget *parent = nullptr);

    void setWidget(QWidget *topWidget, QWidget *leftWidget, QTabWidget *tabWidget, QWidget *highestWidget);

    int addTab(QWidget *widget, const QIcon& icon, const QString &label);
    int insertTab(int index, QWidget *widget, const QIcon& icon, const QString &label);
    void removeTab(int index);
    QString tabText(int index) const;
    QWidget *widget(int index) const;
    int count() const;
    void clear();

    void setButtonImage(QWidget *widget);

signals:

public slots:
    void slotButtonClicked(QAbstractButton * button);

private:
    QWidget *m_topWidget;
    QWidget *m_leftWidget;
    QTabWidget *m_tabWidget;
    QWidget *m_highestWidget;

    QPushButton *m_btnHome;
    QLabel *m_lableHome;
    QPushButton *m_btnParam;
    QLabel *m_lableParam;
    QPushButton *m_btnDoublefreq;
    QLabel *m_lableDoublefreq;
    QPushButton *m_btnControl;
    QLabel *m_lableControl;
    QPushButton *m_btnState;
    QLabel *m_lableState;
    QPushButton *m_btnSenior;

    QMap<QString, QPushButton*> m_mapButton;
    QMap<QString, QLabel*> m_mapLabel;
    QMap<QAbstractButton*, QString> m_mapName;
};

#endif // WORKSPACEWIDGET_H
