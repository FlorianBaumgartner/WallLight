#include "WallLightTest.h"
#include "ui_WallLightTest.h"

#include "../../Firmware/WallLight/src/Engine/DataTypes.hpp"
// #include "../../Firmware/WallLight/src/console.hpp"

#include "../../Firmware/WallLight/src/demo.hpp"

#include "Arduino/console.hpp"


WallLightTest::WallLightTest(QWidget *parent) : QMainWindow(parent), ui(new Ui::WallLightTest)
{
  ui->setupUi(this);

  ui->centralWidget->setStyleSheet("background-color: black;");
  ui->centralWidget->setGeometry(300, 300, WINDOW_WIDTH, WINDOW_HEIGHT);
  ui->centralWidget->setWindowFlags(Qt::WindowStaysOnTopHint);
  ui->centralWidget->setWindowFlags(ui->centralWidget->windowFlags() & ~Qt::WindowStaysOnTopHint);

  label = new QLabel(this);
  label->setGeometry(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  pixmap = new QPixmap(WINDOW_WIDTH, WINDOW_HEIGHT);
  pixmap->fill(Qt::transparent);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &WallLightTest::changeColors);
  timer->start(1000);

  console.printf("Console Test %d\n", 10);
  Demo demo;
  demo.test();
}

WallLightTest::~WallLightTest()
{
  delete label;
  delete pixmap;
  delete timer;
  delete ui;
}

void WallLightTest::on_actionExit_triggered()
{
  this->close();
}

void WallLightTest::changeColors(void)
{
  printf("Test\n");
}

void WallLightTest::paintEvent(QPaintEvent *event)
{
  int height = WINDOW_HEIGHT - PIXELS_OFFSET * 2;
  int hPixel = int(height / PIXELCOUNT);
  int offset = int((WINDOW_HEIGHT - hPixel * PIXELCOUNT) / 2);
  

  QPainter painter(pixmap);
  for (int i = 0; i < PIXELCOUNT; i++)
  {
    painter.fillRect(offset, offset + hPixel * i, 20, hPixel, QColor(0, 255, 0));
  }
  painter.end();

  label->setPixmap(*pixmap);
  label->move(0, 0);
  label->show();
}