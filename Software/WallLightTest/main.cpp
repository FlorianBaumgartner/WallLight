#include "Arduino.h"
#include <QApplication>
#include "WallLightTest.h"


int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    WallLightTest wallLight;
    wallLight.show();

    wallLight.loadGraph("Function.ColorInsert");

    bool res = a.exec();
    wallLight.unloadGraph();
    return res;
}
