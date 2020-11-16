#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cu_header.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <stdio.h>
#include <filesystem>
#include <QMessageBox>
#include <QDoubleSpinBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>

//some external constants
//extern int stop_everything;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //read addresses file
    QFile addFile("addresses_to_plot.cfg");
    if (!addFile.open(QIODevice::ReadOnly))
        QMessageBox::information(this,"info","No addresses_to_plot.cfg file found!");

    QTextStream in(&addFile);

    ui->addressesToPlot->setPlainText(in.readAll());
    addFile.close();
    //start plot file (pipe)
    start_plot();
//    periodSec = new QDoubleSpinBox(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void start_measure(uint64_t gen_lines, int period_milli, int plot_points, std::string add_to_plt)
{
//    stop_everything = 0;
    std::thread data_generator(data_gen_bin, gen_lines);
    std::this_thread::sleep_for(std::chrono::milliseconds(period_milli));
    std::thread counter(keep_counting, period_milli, add_to_plt);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::thread plotter;
    if (add_to_plt.size()>0)    {
        plotter = std::thread(plot, period_milli, plot_points);
    }
    data_generator.join();
    std::cout << "Data_generation finished!" << std::endl;
    stop_everything = 1;
    counter.join();
    if (add_to_plt.size()>0)    {
        plotter.join();
    }

}

void MainWindow::on_startButton_clicked()
{
    //create files folder if not existing
    if (!std::filesystem::exists("files")) {
        std::filesystem::create_directory("files");
    }
    stop_everything = 0;
    ui->resetButton->setEnabled(false);
    ui->periodSec->setReadOnly(true);
    ui->plot_values->setReadOnly(true);
    ui->openAddfileButton->setEnabled(false);
    ui->addressesToPlot->setReadOnly(true);
    int period_milli = static_cast<int>((ui->periodSec->value())*1000.0);
    int plot_points = ui->plot_values->value();

    //write addresses in textedit to addresses_to_plot.cfg
    QFile addFile("addresses_to_plot.cfg");
    if (addFile.open(QIODevice::ReadWrite | QIODevice::Truncate)) {
        QTextStream stream(&addFile);
        stream << ui->addressesToPlot->toPlainText();
        addFile.close();
        }
    else {
        QMessageBox::critical(this, tr("Error"), tr("Can't write addresses_to_plot.cfg"));
        }

    std::thread measure(start_measure, 1000000, period_milli, plot_points, ui->addressesToPlot->toPlainText().QString::toStdString());
    measure.detach();
}

void MainWindow::on_stopButton_clicked()
{
    stop_everything = 1;
    ui->resetButton->setEnabled(true);
    ui->periodSec->setReadOnly(false);
    ui->plot_values->setReadOnly(false);
    ui->openAddfileButton->setEnabled(true);
    ui->addressesToPlot->setReadOnly(false);
}

void MainWindow::on_quitButton_clicked()
{
    QMessageBox::StandardButton exit_reply = QMessageBox::question(this,"Quit Program",
                                     "Would you like to quit?",QMessageBox::Yes | QMessageBox::No);
    if (exit_reply == QMessageBox::Yes) {
        stop_everything = 1;
        end_plot();
        QApplication::quit();
    } else {
    std::cout << "stay here!" << std::endl;
    }
}

void reset()
{
    std::remove("./files/counts.txt");
    std::remove("./files/temp_counts.txt");
    std::remove("./files/data.bin");
    first_line = 0;
}

void MainWindow::on_resetButton_clicked()
{
//    stop_everything = 1;
    reset();
}


void MainWindow::on_openAddfileButton_clicked()
{
    //read addresses file
    QFile addFile(QFileDialog::getOpenFileName(this, tr("Open file with addresses to plot"), "./",
                                               tr("Text Files (*.cfg *.txt)")));
    if (!addFile.open(QIODevice::ReadOnly))
        QMessageBox::critical(this, tr("Error"), tr("Choose a plain text file!"));

    QTextStream in(&addFile);

    ui->addressesToPlot->setPlainText(in.readAll());
    addFile.close();
}
