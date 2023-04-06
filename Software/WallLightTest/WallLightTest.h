#ifndef WALLLIGHT_TEST_H
#define WALLLIGHT_TEST_H

#include <QMainWindow>
#include <QTimer>
#include <QPainter>
#include <QColor>
#include <QPixmap>
#include <QLabel>

#include "Test/TestEngine.hpp"

#include "../../Firmware/WallLight/src/Engine/WallLightConfig.hpp"
#include "../../Firmware/WallLight/src/Engine/DataTypes.hpp"

namespace Ui {
class WallLightTest;
}

class WallLightTest : public QMainWindow
{
    Q_OBJECT

  public:
    explicit WallLightTest(QWidget *parent = 0);
    ~WallLightTest();

    static const int WINDOW_WIDTH = 250;
    static const int WINDOW_HEIGHT = 1000;
    static const int PIXELS_OFFSET = 50;
        
    static const int FRAMERATE = 50;
    static const int PIXELCOUNT = 288;

    

  private slots:
    void on_actionExit_triggered();
    void changeColors(void);
    void paintEvent(QPaintEvent *event);

  private:
    Ui::WallLightTest *ui;
    QTimer* timer;
    QPixmap* pixmap;
    QLabel* label;

    Engine* engine;
    LedVector* output;
    bool running = false;
};

#endif // WALLLIGHT_TEST_H
