#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDoubleSpinBox>

//some external constants
//extern int stop_everything;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_quitButton_clicked();

    void on_resetButton_clicked();

    void on_openAddfileButton_clicked();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
