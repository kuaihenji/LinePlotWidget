#include "LinePlotWidget.h"

LinePlotWidget::LinePlotWidget(QWidget *parent)
    :BaseWidget(parent)
{
    initView();
    connect(this,SIGNAL(signal_LineDataChange(ModelDatas&)),this,SLOT(slot_LineDataChange(ModelDatas&)),Qt::DirectConnection);
    connect(this,SIGNAL(signal_ScatterDataChange(ModelDatas&)),this,SLOT(slot_ScatterDataChange(ModelDatas&)),Qt::DirectConnection);
}

LinePlotWidget::~LinePlotWidget()
{

}

void LinePlotWidget::initView()
{
    mBackWidget = new QWidget(this);
    mBackWidget->resize(width(),height());
    mBackWidget->move(0,0);
    mBackWidget->setAttribute(Qt::WA_StyledBackground,true);
    mBackWidget->setStyleSheet("background-color: #EFEFEF;border : 1px solid #BCBCC2;");




    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("HH:mm");
    dateTicker->setDateTimeSpec(Qt::TimeSpec::LocalTime);

    mPlot = new QCustomPlot(mBackWidget);
    mPlot->move(5,5);
    mPlot->resize(width(),height());
    mPlot->setBackground(QBrush(QColor("#F4F4FB")));


    mPlot->plotLayout()->insertRow(0);
    titleElement = new QCPTextElement(mPlot, "", QFont("Cantarell", 14));

    mPlot->plotLayout()->addElement(0, 0, titleElement);
    mPlot->plotLayout()->setRowStretchFactor(0,0.0001);


    QMargins marg(500,-200,0,10);
    mPlot->legend->setVisible(true);
    mPlot->legend->setBrush(QBrush(Qt::transparent));
    mPlot->legend->setFillOrder(QCPLayoutGrid::foColumnsFirst);
    mPlot->legend->setWrap(6);
    mPlot->legend->setBorderPen(Qt::NoPen);
    //mPlot->legend->setMargins(marg);
    //mPlot->legend->setMinimumMargins(QMargins(0,0,0,0));
//    mPlot->legend->setFont(QFont("Helvetica", 14));
    mPlot->setAntialiasedElement(QCP::AntialiasedElement::aeAxes);

    mPlot->xAxis->setTicker(dateTicker);
    mPlot->xAxis->setSubTickPen(Qt::NoPen);

    mPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    mPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    mPlot->yAxis->setSubTickPen(Qt::NoPen);

//    mPlot->yAxis->setNumberPrecision(4);
//    mPlot->yAxis2->setNumberPrecision(4);


//    mPlot->yAxis->ticker()->setTickCount(10);
//    mPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);
//    mPlot->yAxis2->ticker()->setTickStepStrategy(QCPAxisTicker::tssReadability);



    mTracer = new QCPItemTracer(mPlot);
    mTracer->setStyle(QCPItemTracer::tsNone);
    mTracer->setInterpolating(true);
    connect(mPlot,SIGNAL(mouseMove(QMouseEvent*)),this,SLOT(setToolTip(QMouseEvent*)));
    connect(mPlot->xAxis,SIGNAL(rangeChanged(QCPRange)),this,SLOT(setAxisRange(QCPRange)));

    if(mPlot->legend->visible()){
        int count = mPlot->plotLayout()->rowCount();
        mPlot->plotLayout()->addElement(count,0,mPlot->legend);
        mPlot->plotLayout()->setRowStretchFactor(count,0.0001);
        mPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignRight|Qt::AlignTop);
    }

//    shadowEffect = new QGraphicsDropShadowEffect(this);
//    shadowEffect->setOffset(5, 5);              //阴影的偏移量
//    shadowEffect->setColor(QColor(245,245,245)); //阴影的颜色
//    shadowEffect->setBlurRadius(8);             // 阴影圆角的大小


    mValueLabel = new QLabel(mPlot);

    mValueLabel->setStyleSheet(QString("QLabel{color:#000000; font-size:15px;"
                                        " background-color: rgba(245,245,245,180); border-radius:4px; text-align:center;}"));
    mValueLabel->setFixedSize(180, 60);

    mValueLabel->setAlignment(Qt::AlignLeft);
    mValueLabel->hide();


}

void LinePlotWidget::setAxisRange(const QCPRange &newRange)
{
    QCPAxisTickerDateTime dateTicker;
    mPlot->xAxis->setRangeLower(dateTicker.dateTimeToKey(mStartTime));
}

void LinePlotWidget::slot_LineDataChange(ModelDatas &dataBeans)
{
    double maxValue = 0;
    double minValue = 0;


    for(int index = 0;index < mLineGraphList.size() ;index++){
              mLineGraphList[index]->data().data()->clear();
              qInfo()<<"clear line data ";
    }

//    for(int index = 0;index < mSeriesValueList.size() ;index++){
//          mPlot->graph(index)->data().data()->clear();
//    }

    if(!dataBeans.isEmpty()){

        QCPAxisTickerDateTime dateTicker;
        std::string valueKey = QString(mSeriesValueList.first()).toStdString();
        maxValue = std::atof(dataBeans.first().getValue(valueKey).c_str());
        minValue = maxValue;

        foreach (ModelData bean, dataBeans) {

            for(int index = 0;index < mSeriesValueList.size() ;index++)
            {
                std::string timeStepKey = "date_time";
                valueKey = QString(mSeriesValueList[index]).toStdString();
                QDateTime valueTime = QDateTime::fromString(QString::fromStdString(bean.getValue(timeStepKey)),"yyyy-MM-dd HH:mm:ss");

                std::string sValue = bean.getValue(valueKey);
                if( !sValue.empty() )
                {
                    double valueNumber = std::atof(sValue.c_str());

                    maxValue = maxValue > valueNumber ? maxValue : valueNumber;
                    minValue = minValue < valueNumber ? minValue : valueNumber;

                    mLineGraphList[index]->addData(dateTicker.dateTimeToKey(valueTime),valueNumber);
                }
            }
        }

        maxValue = maxValue + mMax;
        minValue = minValue - mMin;

        titleElement->setText(mLineTitle);
        //qInfo()<<mStartTime.toString("yyyyy-MM-dd HH:mm:ss")<<mEndTime.toString("yyyy-MM-dd HH:mm:ss")<<maxValue<<minValue;
        mPlot->xAxis->setRange(dateTicker.dateTimeToKey(mStartTime),dateTicker.dateTimeToKey(mEndTime));
        mPlot->xAxis->ticker()->setTickCount(mTickCount);
        mPlot->yAxis->setRange(minValue,maxValue);


    }

    mPlot->replot();
}

void LinePlotWidget::slot_ScatterDataChange(ModelDatas &dataBeans)
{
    double maxValue = 0;
    double minValue = 0;

    for(int index = 0;index < mScatterGraphList.size() ;index++){
          mScatterGraphList[index]->data().data()->clear();
    }

    if(!dataBeans.isEmpty()){

        QCPAxisTickerDateTime dateTicker;
        std::string valueKey = QString(mSeriesValueList.first()).toStdString();
        maxValue = std::atof(dataBeans.first().getValue(valueKey).c_str());
        minValue = maxValue;

        foreach (ModelData bean, dataBeans) {

            for(int index = 0;index < mScatterValueList.size() ;index++)
            {
                std::string timeStepKey = "date_time";
                valueKey = QString(mScatterValueList[index]).toStdString();
                QDateTime valueTime = QDateTime::fromString(QString::fromStdString(bean.getValue(timeStepKey)),"yyyy-MM-dd HH:mm:ss");
                double valueNumber = std::atof(bean.getValue(valueKey).c_str());

                maxValue = maxValue > valueNumber ? maxValue : valueNumber;
                minValue = minValue < valueNumber ? minValue : valueNumber;

//                qInfo()<<index<<dateTicker.dateTimeToKey(valueTime)<<valueNumber;
                mScatterGraphList[index]->addData(dateTicker.dateTimeToKey(valueTime),valueNumber);
//                mPlot->graph(index)->addData(dateTicker.dateTimeToKey(valueTime),valueNumber);
            }
        }

        maxValue = maxValue + mMax;
        minValue = minValue - mMin;

        titleElement->setText(mLineTitle);
//        qInfo()<<mStartTime.toString("yyyyy-MM-dd HH:mm:ss")<<mEndTime.toString("yyyy-MM-dd HH:mm:ss");
        mPlot->xAxis->setRange(dateTicker.dateTimeToKey(mStartTime),dateTicker.dateTimeToKey(mEndTime));
        mPlot->xAxis->ticker()->setTickCount(mTickCount);
        mPlot->yAxis->setRange(minValue,maxValue);

        mPlot->replot();
    }
}

void LinePlotWidget::setToolTip(QMouseEvent *e)
{
    QString toopTipString = "";
    QString fixTipString = "";
    QCPAxisTickerDateTime dateTicker;
    QList<double> lineValueList;
    bool showFlag = false;
    //获得鼠标位置处对应的横坐标数据x
    double xPlotPos = mPlot->xAxis->pixelToCoord(e->pos().x());
    double yPlotPos = mPlot->yAxis->pixelToCoord(e->pos().y());

    for(int index =0;index < mLineGraphList.size();index++)
    {
        mTracer->setGraph(mLineGraphList.at(index)); //将游标和该曲线图层想连接
        mTracer->setGraphKey(xPlotPos); //将游标横坐标（key）设置成刚获得的横坐标数据x
        mTracer->updatePosition(); //使得刚设置游标的横纵坐标位置生效
        //以下代码用于更新游标说明的内容
        double xValue = mTracer->position->key();
        double yValue = mTracer->position->value();
        if( mLineGraphList.at(index)->data().data()->isEmpty() ){
            yValue = 0;
        }


        if(toopTipString.isEmpty()){
            toopTipString = QString("<font color = #000000  >&nbsp;&nbsp;时间: %1</font><br>").arg(dateTicker.keyToDateTime(xValue).toString("HH:mm:ss"));
            QString lineString = QObject::tr("<font color = %0  >&nbsp;&nbsp;━</font>").arg(mLineGraphList.at(index)->pen().color().name())+
                                             QString("<font color = #000000  >   %1: %2</font>").arg(mLineGraphList.at(index)->name()).arg(QString::number(yValue,'f',4));
            toopTipString.append(lineString);

            fixTipString = QString("   时间: %1\n").arg(dateTicker.keyToDateTime(xValue).toString("HH:mm:ss"));
            fixTipString.append(QString("    ━ %0: %1").arg(mLineGraphList.at(index)->name()).arg(QString::number(yValue,'f',4)));

        }else {
            toopTipString.append("<br>");
            fixTipString.append("\n");
            fixTipString.append(QString("    ━ %0: %1").arg(mLineGraphList.at(index)->name()).arg(QString::number(yValue,'f',4)));

            QString lineString = QObject::tr("<font color = %0  >&nbsp;&nbsp;━</font>").arg(mLineGraphList.at(index)->pen().color().name())+
                                             QString("<font color = #000000  >   %1: %2</font>").arg(mLineGraphList.at(index)->name()).arg(QString::number(yValue,'f',4));
            toopTipString.append(lineString);
        }

        lineValueList.push_back(yValue);
    }

    double yPosRange = qAbs(mPlot->yAxis->range().upper - mPlot->yAxis->range().lower);
    for(int index =0;index < lineValueList.size();index++)
    {
//        qInfo()<<y<<lineValueList.at(index);
        int iY = (int)yPlotPos;
        int yOffset = yPosRange / 10;
        int iValue = lineValueList.at(index);
        if(iY>= iValue-yOffset && iY <= iValue+yOffset)
        {
            showFlag = true;
        }
    }

    QFontMetrics metrics(QFont("Cantarell",-1));
    QRect textRect = metrics.boundingRect(QRect(0, 0, 300, 300), Qt::AlignLeft, fixTipString);
    mValueLabel->setText(toopTipString);         
    mValueLabel->setFixedSize(textRect.width(),textRect.height());
//    mValueLabel->setText(testString);


//    QPoint pos = this->rect().bottomLeft();
//    QPoint pos2 = this->rect().topRight();

    QPoint pos = mPlot->rect().bottomLeft();
    QPoint pos2 = mPlot->rect().topRight();

    QPoint curPos = mapFromGlobal(QCursor::pos());
    int xPos = curPos.x() - mValueLabel->width() / 2;
    int yPos = curPos.y() - mValueLabel->height() * 1.2;
    if(xPos + mValueLabel->width() > pos2.x()){
        xPos = pos2.x() - mValueLabel->width();
    }
    xPos = xPos > 0 ? xPos : 0;
    yPos = yPos > 0 ? yPos : 0;


    mValueLabel->move(xPos, yPos);
    if(showFlag){
        mValueLabel->show();
    }else {
        mValueLabel->hide();
    }
}

void LinePlotWidget::setPlotDragZoom()
{
    mPlot->setInteractions(QCP::iRangeZoom | QCP::iRangeDrag);
    mPlot->axisRect()->setRangeDrag(Qt::Horizontal);
    mPlot->axisRect()->setRangeZoomFactor(2);
    mPlot->axisRect()->setRangeZoom(Qt::Horizontal);
}

void LinePlotWidget::setLineStartTime(const QDateTime &time)
{
    this->mStartTime = time;
}

void LinePlotWidget::setLineEndTime(const QDateTime &time)
{
    this->mEndTime = time;
}

void LinePlotWidget::setLineMaxValue(const double &value)
{
    this->mMax = value;
}

void LinePlotWidget::setLineMinValue(const double &value)
{
    this->mMin = value;
}

void LinePlotWidget::setLineTitle(const QString &titleString)
{
    this->mLineTitle = titleString;
}

void LinePlotWidget::setTickCount(const int &value)
{
    this->mTickCount = value;
}

void LinePlotWidget::setXaxisAngle(double rotate)
{
    mPlot->xAxis->setTickLabelRotation(rotate);
}

void LinePlotWidget::setYaxisLable(const QString &lableString)
{
    QFont lableFont;
    lableFont.setPixelSize(20);

    mPlot->yAxis->setLabel(lableString);
    mPlot->yAxis->setLabelFont(lableFont);
}



void LinePlotWidget::addOnlyOneLineGraph(const QPair<QString, QString> &linePair, const QString &colorString)
{

    for(int index = 0;index < mLineGraphList.size() ;index++){
              mLineGraphList[index]->data().data()->clear();
    }

    mSeriesNameList.clear();
    mSeriesValueList.clear();
    mPlot->clearGraphs();
    mLineGraphList.clear();

    QString lineName = linePair.first;
    QString lineKey = linePair.second;
    QPen linePen;
    linePen.setWidth(2);

    if(!lineName.isEmpty() && !lineKey.isEmpty())
    {
        int index = mSeriesNameList.size();
        mSeriesNameList<<lineName;
        mSeriesValueList<<lineKey;

        QCPGraph *lineGraph =  mPlot->addGraph(mPlot->xAxis,mPlot->yAxis);

        lineGraph->setName(lineName);
        lineGraph->setLineStyle(QCPGraph::lsLine);
        if(!colorString.isEmpty()){
            linePen.setColor(QColor(colorString));
        }
        lineGraph->setPen(linePen);
//        mPlot->graph(index)->setPen(linePen);
        mLineGraphList.push_back(lineGraph);
    }
}

void LinePlotWidget::addLineGraph(const QPair<QString, QString> &linePair, const QString &colorString)
{
    QString lineName = linePair.first;
    QString lineKey = linePair.second;
    QPen linePen;
    linePen.setWidth(2);

    if(!lineName.isEmpty() && !lineKey.isEmpty())
    {
        int index = mSeriesNameList.size();
        mSeriesNameList<<lineName;
        mSeriesValueList<<lineKey;

        QCPGraph *lineGraph =  mPlot->addGraph(mPlot->xAxis,mPlot->yAxis);

        lineGraph->setName(lineName);
        lineGraph->setLineStyle(QCPGraph::lsLine);
        if(!colorString.isEmpty()){
            linePen.setColor(QColor(colorString));
        }
        lineGraph->setPen(linePen);
//        mPlot->graph(index)->setPen(linePen);
        mLineGraphList.push_back(lineGraph);
    }
}

void LinePlotWidget::addScatterGraph(const QPair<QString, QString> &scatterPair, const QString &colorString)
{
    QString scatterName = scatterPair.first;
    QString scatterKey = scatterPair.second;
    QPen drawPen;
    drawPen.setWidth(2);

    if(!scatterName.isEmpty() && !scatterKey.isEmpty())
    {
        mScatterNameList<<scatterName;
        mScatterValueList<<scatterKey;

        QCPGraph *scatterGraph =  mPlot->addGraph();

        scatterGraph->setName(scatterName);
        scatterGraph->setLineStyle(QCPGraph::lsLine);
        scatterGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc,10));
        if(!colorString.isEmpty()){
            drawPen.setColor(QColor(colorString));
        }
        scatterGraph->setPen(drawPen);
        mScatterGraphList.push_back(scatterGraph);
    }
}

void LinePlotWidget::updateWidgetSize(int w, int h)
{
    //mBackWidget->resize(w,h);
    mPlot->resize(w,h);
}


void LinePlotWidget::resizeEvent(QResizeEvent *event)
{


    mBackWidget->resize(width(),height());
    mPlot->resize(width()-5*2,height()-5*2);
    //mPlot->setFixedSize(width()-20,height()-20);

    //qInfo()<<mPlot->width()<<mPlot->height();
}
