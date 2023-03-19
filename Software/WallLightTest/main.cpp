#include "WallLightTest.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WallLightTest wallLight;
    wallLight.show();

    return a.exec();
}
