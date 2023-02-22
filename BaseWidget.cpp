#include "BaseWidget.h"

BaseWidget::BaseWidget(QWidget *parent) : QWidget(parent)
{
}

BaseWidget::~BaseWidget()
{
}

void BaseWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter Painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &Painter, this);
}

