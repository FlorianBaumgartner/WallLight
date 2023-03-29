#include "WallLightTest.h"
#include "ui_WallLightTest.h"
#include "Arduino/console.hpp"

// #include "../../Firmware/WallLight/src/Engine/Utility.hpp"


WallLightTest::WallLightTest(QWidget *parent) : QMainWindow(parent), ui(new Ui::WallLightTest)
{
  ui->setupUi(this);
  running = true;

  WallLightConfig::framerate(FRAMERATE);
  WallLightConfig::pixelcount(PIXELCOUNT);
  output = new LedVector(true);

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
  timer->start(1000 / FRAMERATE);

  console.printf("Console Test %d\n", 10);
  console.ok.println("OK");
}

WallLightTest::~WallLightTest()
{
  running = false;
  delete label;
  delete pixmap;
  delete timer;
  delete ui;
  delete output;
}

void WallLightTest::on_actionExit_triggered()
{
  running = false;
  timer->stop();
  delay(100);
  this->close();
}

void WallLightTest::changeColors(void)
{
  // console.printf("Time: %f\n", Utility::rand());
  output->fill((millis() % 1000) / 1000.0); // update output from test environment
  ui->centralWidget->update();
}

void WallLightTest::paintEvent(QPaintEvent *event)
{
  if(!running)
     return;
  int height = WINDOW_HEIGHT - PIXELS_OFFSET * 2;
  int hPixel = int(height / PIXELCOUNT);
  int offset = int((WINDOW_HEIGHT - hPixel * PIXELCOUNT) / 2);
  
  QPainter painter(pixmap);
  for (int i = 0; i < PIXELCOUNT; i++)
  {
    uint8_t r = constrain(output->value[WallLightConfig::LED_R][i], 0.0, 1.0) * 255;
    uint8_t g = constrain(output->value[WallLightConfig::LED_G][i], 0.0, 1.0) * 255;
    uint8_t b = constrain(output->value[WallLightConfig::LED_B][i], 0.0, 1.0) * 255;
    painter.fillRect(offset, offset + hPixel * i, 20, hPixel, QColor(r, g, b));
  }
  painter.end();

  label->setPixmap(*pixmap);
  label->move(0, 0);
  label->show();
}