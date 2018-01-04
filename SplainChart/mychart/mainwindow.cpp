#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    id1 = startTimer(500);
    //查找可用的串口
    foreach(const QSerialPortInfo &info , QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        ui->comboBox->addItem(serial.portName());
    }
    this->paintCurve();
}

void MainWindow::paintCurve()
{
    //构建Series，做为图标的数据源，为其添加相应坐标

   // series->setName("spline");
    QPen green(Qt::red);
    green.setWidth(3);
    series->setPen(green);

    //画点
    for(int i = 0;i < 200 ; i++)
    {
        series->append(i,0);
    }
    series->setUseOpenGL(true);//使用OPENGL加速

   /* series->append(0, 2);
    series->append(2, 4);
    series->append(3, 8);
    series->append(7, 4);
    series->append(10, 5);*/
   // *series << QPointF(11, 1) << QPointF(13, 3) << QPointF(17, 6) << QPointF(18, 3) << QPointF(20, 2);

    //坐标
    chart = new QtCharts::QChart;   //构建图标
    chart->legend()->hide();        //隐藏图例
    chart->addSeries(series);       //将数据添加至图表中
    chart->setTitle("简易示波器");
    chart->createDefaultAxes();     //基于图标中的series来创轴


    /*chart->axisY()->setRange(0, 255 );
    chart->axisX()->setRange(0,199);
    chart->axisX()->setTitleText("Time");
    chart->axisY()->setTitleText("幅值(V)");
    */
    axisX->setMin(0);
    axisX->setMin(199);
    axisY->setMin(0);
    axisY->setMax(255);
    switch(ui->comboBox_7->currentIndex())
    {
        case 0:     //1v/div
        {
            axisY->append("-4V", 0);
            axisY->append("-3V", 32);
            axisY->append("-2V", 64);
            axisY->append("-1V", 96);
            axisY->append("0V", 128);
            axisY->append("1V", 160);
            axisY->append("2V", 192);
            axisY->append("3V", 224);
            axisY->append("4V", 255);
        } break;
    case 1:         //0.1v/div
        {
            axisY->append("-0.4V", 0);
            axisY->append("-0.3V", 32);
            axisY->append("-0.2V", 64);
            axisY->append("-0.1V", 96);
            axisY->append("0.0V", 128);
            axisY->append("0.1V", 160);
            axisY->append("0.2V", 192);
            axisY->append("0.3V", 224);
            axisY->append("0.4V", 255);
        }break;
    default:break;
    }



    axisX->append("0ms", 0);
    axisX->append("20ms", 20);
    axisX->append("40ms", 40);
    axisX->append("60ms", 60);
    axisX->append("80ms", 80);
    axisX->append("100ms", 100);
    axisX->append("120ms", 120);
    axisX->append("140ms", 140);
    axisX->append("160ms", 160);
    axisX->append("180ms", 180);
    axisX->append("199ms", 199);

    axisX->setTitleText("时间");
    axisY->setTitleText("幅值(V)");
    //  chartView->chart()->setAxisY(axisY, series);

    chartView = new QtCharts::QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->chart()->setAxisY(axisY,series);
    chartView->chart()->setAxisX(axisX,series);
    ui->verticalLayout->addWidget(chartView);
  //  QMainWindow window;
  //  window.setCentralWidget(chartView);
  //  window.resize(500, 500);
   // window.show();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//定时器

void MainWindow::timerEvent(QTimerEvent *event)
{
    static int i = 0;
    //QByteArray a = ui->comboBox_6->currentIndex();
    if(event->timerId() == id1)
    {
        if(ui->pushButton->text() == tr("关闭串口"))
        {
            serial->write("A");
            switch(ui->comboBox_6->currentIndex())
            {
                case 0:serial->write("0");break;
                case 1:serial->write("1");break;
                case 2:serial->write("2");break;
                default:break;
            }
            switch(ui->comboBox_7->currentIndex())
            {
                case 0:serial->write("0");break;
                case 1:serial->write("1");break;
               // case 2:serial->write("2");break;
                default:break;
            }
            switch(ui->comboBox_8->currentIndex())
            {
                case 0:serial->write("0");break;
                case 1:serial->write("1");break;
                //case 2:serial->write("2");break;
                default:break;
            }
        }
        //qDebug()<<"Timer1" << i++;
    }
}

//读取接受的数据
void MainWindow::Read_Data()
{
    QByteArray buf;
    bool ok = true;
    buf = serial->readAll();

    int x = 0 , y = 0;
    qDebug()<< buf;
    series->pointsVector();
    QList<QByteArray> datalist=buf.split(',');
    for(int i = 0;i < datalist.length();i++)
    {
        if(!datalist[i].isEmpty())
        {
            QList<QByteArray>littleList=datalist[i].split('.');
            //int index = littleList[0].toInt(&ok,10) - 1;
            for(int a = 0; a < littleList.length();a++)
            {
            x = littleList[0].toInt(&ok);
            if(a == 1)
            {
                y = littleList[1].toInt(&ok);
                series->replace(x,x,y);
            }
            }


        }
    }
}

void MainWindow::on_pushButton_clicked()
{
   // qDebug()<<ui->comboBox_3->currentIndex();
    if(ui->pushButton->text() == tr("打开串口"))
    {
        serial = new QSerialPort;
        serial ->setPortName(ui->comboBox->currentText());
        //打开串口
        serial->open(QIODevice::ReadWrite);
        //设置波特率
        serial->setBaudRate(ui->comboBox_2->currentText().toInt());
        //设置数据位数
        switch(ui->comboBox_3->currentIndex())
        {
            case 0:serial->setDataBits(QSerialPort::Data8);
                 break;
            case 1:serial->setDataBits(QSerialPort::Data7);
                 break;
            case 2:serial->setDataBits(QSerialPort::Data6);
                 break;
            default:break;
        }
        //设置奇偶校验
        switch(ui->comboBox_4->currentIndex())
        {
            case 0:serial->setParity(QSerialPort::NoParity);
                break;
            case 1:serial->setParity(QSerialPort::OddParity);
                break;
            case 2:serial->setParity(QSerialPort::EvenParity);
                break;

        }
        //设置停止位
        switch(ui->comboBox_5->currentIndex())
        {
            case 0:serial->setStopBits(QSerialPort::OneStop);
                break;
            case 1:serial->setStopBits(QSerialPort::TwoStop);
                break;
            default:break;
        }
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //关闭设置菜单使能
        ui->comboBox->setEnabled(false);
        ui->comboBox_2->setEnabled(false);
        ui->comboBox_3->setEnabled(false);
        ui->comboBox_4->setEnabled(false);
        ui->comboBox_5->setEnabled(false);
        ui->pushButton->setText(tr("关闭串口"));
        //连接信号槽
        QObject::connect(serial,&QSerialPort::readyRead,this,&MainWindow::Read_Data);
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        //恢复设置使能
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        ui->pushButton->setText(tr("打开串口"));
        QObject::disconnect(serial,SIGNAL(readyRead()),this,SLOT(Read_Data()));
    }
}

//垂直灵敏度改变
void MainWindow::on_comboBox_7_currentIndexChanged(const QString &arg1)
{
   // qDebug()<< ui->comboBox_7->currentIndex();
    switch(ui->comboBox_7->currentIndex())
    {
        case 0:     //1v/div
        {
            axisY->replaceLabel("0.4V" , "4V" );
            axisY->replaceLabel("0.3V" , "3V"  );
            axisY->replaceLabel("0.2V" , "2V"  );
            axisY->replaceLabel("0.1V" , "1V"  );
            axisY->replaceLabel("0.0V"  , "0V" );
            axisY->replaceLabel("-0.1V"  , "-1V");
            axisY->replaceLabel("-0.2V"  , "-2V");
            axisY->replaceLabel("-0.3V"  , "-3V");
            axisY->replaceLabel("-0.4V"  , "-4V");
        } break;
    case 1:         //0.1v/div
        {
            axisY->replaceLabel( "4V" , "0.4V");
            axisY->replaceLabel( "3V" , "0.3V");
            axisY->replaceLabel( "2V" , "0.2V");
            axisY->replaceLabel( "1V" , "0.1V");
            axisY->replaceLabel( "0V" , "0.0V" );
            axisY->replaceLabel( "-1V" ,"-0.1V");
            axisY->replaceLabel( "-2V" ,"-0.2V");
            axisY->replaceLabel( "-3V" ,"-0.3V");
            axisY->replaceLabel( "-4V" ,"-0.4V" );
        }break;
    default:break;
    }
}

//扫描速度改变
void MainWindow::on_comboBox_6_currentIndexChanged(const QString &arg1)
{
    //qDebug() << ui->comboBox_6->currentIndex();
    switch(ui->comboBox_6->currentIndex())
    {
        case 0:     //20ms
        {
            axisX->replaceLabel("0us"   , "0ms"   );
            axisX->replaceLabel("2us"   , "20ms"  );
            axisX->replaceLabel("4us"   , "40ms"  );
            axisX->replaceLabel("6us"   , "60ms"  );
            axisX->replaceLabel("8us"   , "80ms"  );
            axisX->replaceLabel("10us"  , "100ms" );
            axisX->replaceLabel("12us"  , "120ms" );
            axisX->replaceLabel("14us"  , "140ms" );
            axisX->replaceLabel("16us"  , "160ms" );
            axisX->replaceLabel("18us"  , "180ms" );
            axisX->replaceLabel("20us"  , "199ms" );

            axisX->replaceLabel("0ns"    , "0ms"  );
            axisX->replaceLabel("100ns"   ,"20ms"  );
            axisX->replaceLabel("200ns"   ,"40ms"  );
            axisX->replaceLabel("300ns"   ,"60ms"  );
            axisX->replaceLabel("400ns"   ,"80ms"  );
            axisX->replaceLabel("500ns"  , "100ms");
            axisX->replaceLabel("600ns"  , "120ms");
            axisX->replaceLabel("700ns"  , "140ms");
            axisX->replaceLabel("800ns"  , "160ms");
            axisX->replaceLabel("900ns"  , "180ms");
            axisX->replaceLabel("999ns"  , "199ms");
        }break;
        case 1://2us
        {
            axisX->replaceLabel("0ms"   , "0us"  );
            axisX->replaceLabel("20ms"  , "2us"  );
            axisX->replaceLabel("40ms"  , "4us"  );
            axisX->replaceLabel("60ms"  , "6us"  );
            axisX->replaceLabel("80ms"  , "8us"  );
            axisX->replaceLabel("100ms" , "10us" );
            axisX->replaceLabel("120ms" , "12us" );
            axisX->replaceLabel("140ms" , "14us" );
            axisX->replaceLabel("160ms" , "16us" );
            axisX->replaceLabel("180ms" , "18us" );
            axisX->replaceLabel("199ms" , "20us" );

            axisX->replaceLabel("0ns"    , "0us" );
            axisX->replaceLabel("100ns"  , "2us" );
            axisX->replaceLabel("200ns"  , "4us" );
            axisX->replaceLabel("300ns"  , "6us" );
            axisX->replaceLabel("400ns"  , "8us" );
            axisX->replaceLabel("500ns"  , "10us");
            axisX->replaceLabel("600ns"  , "12us");
            axisX->replaceLabel("700ns"  , "14us");
            axisX->replaceLabel("800ns"  , "16us");
            axisX->replaceLabel("900ns"  , "18us");
            axisX->replaceLabel("999ns"  , "20us");
        }break;

    case 2://100ns
    {
        axisX->replaceLabel( "0ms"   , "0ns"   );
        axisX->replaceLabel( "20ms"  , "100ns" );
        axisX->replaceLabel( "40ms"  , "200ns" );
        axisX->replaceLabel( "60ms"  , "300ns" );
        axisX->replaceLabel( "80ms"  , "400ns" );
        axisX->replaceLabel( "100ms" , "500ns" );
        axisX->replaceLabel( "120ms" , "600ns" );
        axisX->replaceLabel( "140ms" , "700ns" );
        axisX->replaceLabel( "160ms" , "800ns" );
        axisX->replaceLabel( "180ms" , "900ns" );
        axisX->replaceLabel( "199ms" , "999ns" );

        axisX->replaceLabel("0us"   , "0ns"   );
        axisX->replaceLabel("2us"   , "100ns" );
        axisX->replaceLabel("4us"   , "200ns" );
        axisX->replaceLabel("6us"   , "300ns" );
        axisX->replaceLabel("8us"   , "400ns" );
        axisX->replaceLabel("10us"  , "500ns" );
        axisX->replaceLabel("12us"  , "600ns" );
        axisX->replaceLabel("14us"  , "700ns" );
        axisX->replaceLabel("16us"  , "800ns" );
        axisX->replaceLabel("18us"  , "900ns" );
        axisX->replaceLabel("20us"  , "999ns" );
    }
    default : break;
    }
}
