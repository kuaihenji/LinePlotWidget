#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QEvent>
#include <QStyleOption>
#include <QPainter>
#include <QDebug>

class BaseWidget : public QWidget
{
public:
    explicit BaseWidget(QWidget *parent = nullptr);

    virtual ~BaseWidget();

protected:
    void paintEvent(QPaintEvent *event);

    virtual void initView() = 0;

};

#endif // BASEWIDGET_H
