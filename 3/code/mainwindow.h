#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plotarea.h"
#include "logwidget.h"
using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum class CurrentAlgo
{
    None,
    BresenhamLine,
    NaiveLine,
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bline_clicked();

    void on_nline_clicked();

    void on_drawButton_clicked();

    void on_scaleButton_clicked();

private:
    void DisableInput();
    void EnableInputLine();
    void EnableInputCircle();
    void BresenhamLine(int x1, int y1, int x2, int y2);
    void NaiveLine(int x1, int y1, int x2, int y2);
    QString pointF(qreal x, qreal y);
    QString point(int x, int y);
    Ui::MainWindow *ui;
    PlotArea *area = nullptr;
    LogWidget *log = nullptr;
    CurrentAlgo algo = CurrentAlgo::None;
};
#endif
