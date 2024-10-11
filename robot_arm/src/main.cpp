#include "robot_arm.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    robot_arm w;
    w.show();
    return a.exec();
}
