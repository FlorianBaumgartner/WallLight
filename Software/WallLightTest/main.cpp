#include "Arduino.h"
#include <QApplication>
#include "WallLightTest.h"


int main(int argc, char *argv[])
{   
    QApplication a(argc, argv);
    WallLightTest wallLight;
    wallLight.show();

    wallLight.loadGraph("Example.Techno2");
    // wallLight.loadGraph("Example.HeartBeat");
    // wallLight.loadGraph("Function.Brightness");

    bool res = a.exec();
    wallLight.unloadGraph();
    return res;
}
