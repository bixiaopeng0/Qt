#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplineSeries>
#include <QChart>
#include <QChartView>
#include <QLayout>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void paintCurve();
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_pushButton_clicked();
    void Read_Data();
    void on_comboBox_7_currentIndexChanged(const QString &arg1);

    void on_comboBox_6_currentIndexChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    int id1;
    QtCharts::QChart *chart;
    QtCharts::QChartView *chartView;
    QtCharts:: QCategoryAxis *axisY = new QtCharts::QCategoryAxis;
    QtCharts:: QCategoryAxis *axisX = new QtCharts::QCategoryAxis;
    QtCharts::QSplineSeries *series = new QtCharts::QSplineSeries();
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
