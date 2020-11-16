#include "mainwindow.h"
#include "cu_header.h"
#include <QApplication>

//#include <vector>

//some external constants
//extern int stop_everything;
int stop_everything = 0;
unsigned int first_line = 0;
//std::vector<int> add_to_plot(100);
//add_to_plot.push_back(1);
//add_to_plot.push_back(2);
//add_to_plot.push_back(4);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();




    return a.exec();
}
