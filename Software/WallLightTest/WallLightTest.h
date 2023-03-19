#ifndef WALLLIGHT_TEST_H
#define WALLLIGHT_TEST_H

#include <QMainWindow>
#include <QTimer>
#include <QPainter>
#include <QColor>
#include <QPixmap>
#include <QLabel>

namespace Ui {
class WallLightTest;
}

class WallLightTest : public QMainWindow
{
    Q_OBJECT

  public:
    explicit WallLightTest(QWidget *parent = 0);
    ~WallLightTest();

    const int WINDOW_WIDTH = 250;
    const int WINDOW_HEIGHT = 1000;
    const int PIXELS_OFFSET = 50;
        
    const int FRAMERATE = 50;
    const int PIXELCOUNT = 288;

  private slots:
    void on_actionExit_triggered();
    void changeColors(void);
    void paintEvent(QPaintEvent *event);

  private:
    Ui::WallLightTest *ui;
    QTimer* timer;
    QPixmap* pixmap;
    QLabel* label;
};

#endif // WALLLIGHT_TEST_H
