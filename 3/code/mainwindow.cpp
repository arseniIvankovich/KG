#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QValidator>
#include <chrono>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    area = new PlotArea();
    log = new LogWidget();
    setStyleSheet("background-color: white;");

    QGridLayout *g = new QGridLayout;

    g -> addWidget(area, 0, 0, 12, 4);
    g -> addWidget(log, 12, 0, 2, 8);

    g -> addWidget(ui -> nline, 0, 4, 1, 4);
    g -> addWidget(ui -> bline, 1, 4, 1, 4);


    g -> addWidget(ui -> x1, 5, 4, 1, 1, Qt::AlignRight);
    g -> addWidget(ui -> fx, 5, 5, 1, 1);
    g -> addWidget(ui -> y1, 5, 6, 1, 1, Qt::AlignRight);
    g -> addWidget(ui -> fy, 5, 7, 1, 1);

    g -> addWidget(ui -> x2, 6, 4, 1, 1, Qt::AlignRight);
    g -> addWidget(ui -> sx, 6, 5, 1, 1);
    g -> addWidget(ui -> y2, 6, 6, 1, 1, Qt::AlignRight);
    g -> addWidget(ui -> sy, 6, 7, 1, 1);


    g -> addWidget(ui -> drawButton, 11, 4, 1, 2);
    g -> addWidget(ui -> scaleButton, 11, 6, 1, 2);
    for (int i = 0; i < 8; ++i)
    {
        g -> setColumnStretch(i, 10);
    }
    for (int i = 0; i < 14; ++i)
    {
        g -> setRowStretch(i, 10);
    }
    centralWidget() -> setLayout(g);
    setMinimumSize(700, 600);
    resize(800, 700);
    setWindowTitle("Растеризация");
    DisableInput();

    QIntValidator *validator = new QIntValidator(-100, 100, this);
    ui -> fx -> setValidator(validator);
    ui -> fy -> setValidator(validator);
    ui -> sx -> setValidator(validator);
    ui -> sy -> setValidator(validator);
    ui -> fx -> setText("0");
    ui -> fy -> setText("0");
    ui -> sx -> setText("0");
    ui -> sy -> setText("0");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete area;
    delete log;
}
void MainWindow::DisableInput()
{
    ui -> x1 -> setVisible(false);
    ui -> fx -> setVisible(false);
    ui -> y1 -> setVisible(false);
    ui -> fy -> setVisible(false);
    ui -> x2 -> setVisible(false);
    ui -> sx -> setVisible(false);
    ui -> y2 -> setVisible(false);
    ui -> sy -> setVisible(false);

}
void MainWindow::EnableInputLine()
{
    DisableInput();
    ui -> x1 -> setVisible(true);
    ui -> fx -> setVisible(true);
    ui -> y1 -> setVisible(true);
    ui -> fy -> setVisible(true);
    ui -> x2 -> setVisible(true);
    ui -> sx -> setVisible(true);
    ui -> y2 -> setVisible(true);
    ui -> sy -> setVisible(true);
}

QString MainWindow::point(int x, int y)
{
    return "(" + QString::number(x) + ", " + QString::number(y) + ")";
}

void MainWindow::NaiveLine(int x1, int y1, int x2, int y2)
{
    log -> AppendMessage("Пошаговый алгоритм начал работу.");
    auto begin =  chrono::steady_clock::now();
    log -> AppendMessage("Рисуем линию от " + point(x1, y1) + " до " +
                point(x2, y2));
    area -> Clear();
    if (x1 > x2)
    {
         swap(x1, x2);
         swap(y1, y2);
    }
    int dx = x2 - x1;
    int dy = y2 - y1;
    log -> AppendMessage("Вычисленный dx = " + QString::number(dx));
    log -> AppendMessage("Вычисленный dy = " + QString::number(dy));
    if (dx == 0 && dy == 0)
    {
         log -> AppendMessage("dx = 0, dy = 0 => рисуем одну точку " + point(x2, y2));
         area->AddPixel(x1, y1);
    }
    else
    {
         if (abs(dx) >  abs(dy))
         {
             log -> AppendMessage("|dx| > |dy| => рисуем отрезок, просматривая значения x от " + QString::number(x1) +  " до " + QString::number(x2));
             for(int x = x1; x <= x2; ++x)
             {
                 qreal temp = y1 + dy * (x - x1) / (qreal)dx;
                 log -> AppendMessage("Точное значение y для x = " + QString::number(x) + " равно " + QString::number(temp) +
                                      ", рисуем точку " + point(x, temp), 1);
                 area->AddPixel(x, (int)temp);
             }
         }
         else
         {
             if (y1 > y2)
             {
                  swap(x1, x2);
                  swap(y1, y2);
             }
             log -> AppendMessage("|dy| >= |dx| => рисуем отрезок, просматривая значения y от " + QString::number(y1) +  " до " + QString::number(y2));
             for (int y = y1; y <= y2; ++y)
             {
                 qreal temp = dx / (qreal)dy * (y - y1) + x1;
                 log -> AppendMessage("Точное значение x для y = " + QString::number(y) + " равно " + QString::number(temp) +
                                      ", рисуем точку " + point(temp, y), 1);
                 area->AddPixel((int)temp, y);
             }
         }
    }
    auto end =  chrono::steady_clock::now();
    int duration =  chrono::duration_cast< chrono::milliseconds>(end - begin).count();
    log -> AppendMessage("Пошаговый алгоритм закончил работу(" + QString::number(duration) + " ms)");
    log -> AppendSeparator();
}
void MainWindow::BresenhamLine(int x1, int y1, int x2, int y2)
{
    log -> AppendMessage("Алгоритм Брезенхема начал работу");
    auto begin =  chrono::steady_clock::now();
    area -> Clear();
    int dx =  abs(x2 - x1);
    log -> AppendMessage("|dx| = " + QString::number(dx));
    int sx = x1 < x2 ? 1 : -1;
    int dy = - abs(y2 - y1);
    log -> AppendMessage("-|dy| = " + QString::number(dy));
    int sy = y1 < y2 ? 1 : -1;
    int error = dx + dy;
    log -> AppendMessage("Рисуем линию от " + point(x1, y1) + " до " +
                point(x2, y2));
    log -> AppendMessage("Пусть линия задается уравнением f(X, Y) = 0. Будем поддерживать f(Xi, Yi) - f(Xi - 1, Yi - 1) в качестве значения ошибки");
    while (true)
    {
        log -> AppendMessage("Рисуем точку " + point(x1, y1));
        area->AddPixel(x1, y1);
        log -> AppendMessage("error = " + QString::number(error), 1);
        if (x1 == x2 && y1 == y2)
        {
            log -> AppendMessage("Достигнута точка " + point(x2, y2) + ", выход из цикла", 1);
            break;
        }
        int e2 = 2 * error;
        if (e2 >= dy)
        {
            if (x1 == x2)
            {
                log -> AppendMessage("Достигнут x1, выход из цикла", 1);
                break;
            }
            log -> AppendMessage("error - 0.5dy >= 0, значит сдвигаем текущий x на " + QString::number(sx) +
                                 ", значение ошибки уменьшается на " + QString::number( abs(dy)), 1);
            error = error + dy;
            x1 = x1 + sx;
        }
        if (e2 <= dx)
        {
            if (y1 == y2)
            {
                log -> AppendMessage("Достигнут y1, выход из цикла", 1);
                break;
            }
            log -> AppendMessage("error - 0.5dx <= 0, значит сдвигаем текущий y на " + QString::number(sy) +
                                 ", значение ошибки увеличивается на " + QString::number(dx), 1);
            error = error + dx;
            y1 = y1 + sy;
        }
    }
    auto end =  chrono::steady_clock::now();
    int duration =  chrono::duration_cast< chrono::milliseconds>(end - begin).count();
    log -> AppendMessage("Алгоритм Брезенхема закончил работу(" + QString::number(duration) + " ms)");
    log -> AppendSeparator();
}

void MainWindow::on_bline_clicked()
{
    area -> Clear();
    algo = CurrentAlgo::BresenhamLine;
    EnableInputLine();
}




void MainWindow::on_nline_clicked()
{
    area -> Clear();
    algo = CurrentAlgo::NaiveLine;
    EnableInputLine();
}



void MainWindow::on_drawButton_clicked()
{
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    auto validateField = [](QLineEdit* ed){
        if (ed -> text().isEmpty())
        {
            ed -> setText("0");
        }
    };
    validateField(ui -> fx);
    validateField(ui -> fy);
    validateField(ui -> sx);
    validateField(ui -> sy);
    switch(algo)
    {
        case CurrentAlgo::None:
            QMessageBox::warning(centralWidget(), "Ошибка", "Нечего рисовать");
            break;
        case CurrentAlgo::BresenhamLine:
            x0 = ui -> fx -> text().toInt();
            y0 = ui -> fy -> text().toInt();
            x1 = ui -> sx -> text().toInt();
            y1 = ui -> sy -> text().toInt();
            BresenhamLine(x0, y0, x1, y1);
            break;
        case CurrentAlgo::NaiveLine:
            x0 = ui -> fx -> text().toInt();
            y0 = ui -> fy -> text().toInt();
            x1 = ui -> sx -> text().toInt();
            y1 = ui -> sy -> text().toInt();
            NaiveLine(x0, y0, x1, y1);
            break;
    }
}


void MainWindow::on_scaleButton_clicked()
{
    int length =  min(width(), height());
    bool ok;
    int nu = QInputDialog::getInt(centralWidget(), "Изменение параметров", "Введите новое значение единичного отрезка", area -> getScale(), 3, length / 10, 1, &ok);
    if (ok)
    {
        area -> SetUnit(nu);
        area -> repaint();
    }
}

