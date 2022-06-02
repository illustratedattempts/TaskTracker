#include "tasktracker.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TaskTracker gui;
    gui.show();

    return a.exec();
}
