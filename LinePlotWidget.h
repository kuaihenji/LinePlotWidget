#ifndef LINEPLOTWIDGET_H
#define LINEPLOTWIDGET_H

#include <QPair>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>

#include <QStackedLayout>
#include "BaseWidget.h"
#include "ModelData.h"
#include "qcustomplot.h"


/*使用注意:
   1.添加曲线（或散点）                                               addLineGraph( QPair<QString,QString>("字段注释","字段名称"),"16进制颜色" )、addScatterGraph()
   2.设置曲线的开始，结束时间（决定图表X轴的跨度）    setLineStartTime、setLineEndTime
   3.设置曲线Y轴最大、最小偏移值                                 setLineMaxValue、setLineMinValue
   4.传入数据到图表                                                      signal_LineDataChange、signal_ScatterDataChange
可选：
    设置图表可拖拽缩放                                                  setPlotDragZoom
    设置X轴可见刻度                                                      setTickCount
    设置图表标题                                                           setLineTitle
*/
class LinePlotWidget : public BaseWidget
{
    Q_OBJECT
public:
    explicit LinePlotWidget(QWidget *parent = nullptr);
    ~LinePlotWidget();

    void setPlotDragZoom();

    void setLineStartTime(const QDateTime &time);

    void setLineEndTime(const QDateTime &time);

    void setLineMaxValue(const double &value);

    void setLineMinValue(const double &value);

    void setLineTitle(const QString &titleString);

    void setTickCount(const int &value);

    void setXaxisAngle(double rotate);

    void setYaxisLable(const QString &lableString);

    void setLineTypeHidden(bool flag);

    void addOnlyOneLineGraph(const QPair<QString,QString> &linePair, const QString &colorString = "");

    void addLineGraph(const QPair<QString,QString> &linePair, const QString &colorString = "");

    void addScatterGraph(const QPair<QString,QString> &scatterPair, const QString &colorString = "");

    void updateWidgetSize(int w,int h);


signals:
    void signal_LineDataChange(ModelDatas &dataBeans);

    void signal_ScatterDataChange(ModelDatas &dataBeans);

protected:
    void resizeEvent(QResizeEvent *event);

    void initView();

public slots:
    void setAxisRange(const QCPRange &newRange);

    void slot_LineDataChange(ModelDatas &dataBeans);

    void slot_ScatterDataChange(ModelDatas &dataBeans);

    void setToolTip(QMouseEvent *e);

private:
    QWidget *mBackWidget;
    QGraphicsDropShadowEffect *shadowEffect;
    QLabel *mValueLabel;
    QCPTextElement *titleElement;
    QCustomPlot *mPlot;
    QCPItemTracer *mTracer;

    QList<QCPGraph*> mLineGraphList;
    QStringList mSeriesNameList;
    QStringList mSeriesValueList;

    QList<QCPGraph*> mScatterGraphList;
    QStringList mScatterNameList;
    QStringList mScatterValueList;

    QString mLineTitle;
    QDateTime mStartTime;
    QDateTime mEndTime;

    double mMax;
    double mMin;

    int mTickCount;
};

#endif // LINEPLOTWIDGET_H
