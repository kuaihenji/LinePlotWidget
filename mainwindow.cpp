#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{


    pWidget = new LinePlotWidget(this);
    pWidget->move(0,0);
    //mLineDatasPage->setPlotDragZoom();

    pWidget->addLineGraph(QPair<QString,QString>("Value1","value1"),"#4169E1");
    pWidget->addLineGraph(QPair<QString,QString>("Value2","value2"),"#FFA500");

    ModelDatas lineDatas;

    QDateTime currentDateTime = QDateTime(QDate::currentDate(),QTime(0,0,0,0));
    QDateTime nextDateTime = currentDateTime.addDays(1);
    QDateTime tempDateTime = currentDateTime;
    while ( tempDateTime <= nextDateTime )
    {
        ModelData clData;
        double dValue1 = tempDateTime.time().hour() + tempDateTime.time().minute() * 10;
        double dValue2 = tempDateTime.time().hour() * tempDateTime.time().minute() + tempDateTime.time().second();
        clData.setValue("value1",std::to_string(dValue1));
        clData.setValue("value2",std::to_string(dValue2));
        clData.setValue("date_time",tempDateTime.toString("yyyy-MM-dd HH:mm:ss").toStdString());
        lineDatas.push_back(clData);

        tempDateTime = tempDateTime.addSecs(1*60);
    }


    QString sLineTitle = QDate::currentDate().toString("yyyy-MM-dd")+" curve";

    pWidget->setLineTitle(sLineTitle);
    pWidget->setLineStartTime(currentDateTime.addSecs(-60*30));
    pWidget->setLineEndTime(currentDateTime.addDays(1));
    pWidget->setLineMaxValue(50);
    pWidget->setLineMinValue(50);
    pWidget->setTickCount(18);

    pWidget->signal_LineDataChange(lineDatas);


}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    pWidget->resize(width(),height());
}
