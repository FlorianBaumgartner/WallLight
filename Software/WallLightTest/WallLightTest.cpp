#include "WallLightTest.h"
#include "ui_WallLightTest.h"
#include "Arduino/console.hpp"

// #include "../../Firmware/WallLight/src/Engine/Utility.hpp"

#include "Test/Function/TestFunctionRect.hpp"
#include "Test/TestEngine.hpp"

WallLightTest::WallLightTest(QWidget *parent) : QMainWindow(parent), ui(new Ui::WallLightTest)
{
  ui->setupUi(this);
  running = true;

  WallLightConfig::framerate(FRAMERATE);
  WallLightConfig::pixelcount(PIXELCOUNT);
  output = new LedVector(true);

  ui->centralWidget->setStyleSheet("background-color: #111111;");
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

  engine = new Engine();
  testEngine = new TestEngine(engine);
  console.log.println("[WALLLIGHT_TEST] Setup Environment done");
}

WallLightTest::~WallLightTest()
{
  running = false;
  engine->unloadGraph();

  delete label;
  delete pixmap;
  delete timer;
  delete ui;
  delete testEngine;
  delete engine;
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
    if(!running)
     return;
  int height = WINDOW_HEIGHT - PIXELS_OFFSET * 2;
  int hPixel = int(height / PIXELCOUNT);
  int offset = int((WINDOW_HEIGHT - hPixel * PIXELCOUNT) / 2);

  if(unloadingGraphPending)
  {
    unloadingGraphPending = false;
    console.log.println("[WALLLIGHT_TEST] Unloading graph from Engine");
    engine->unloadGraph();
  }

  if(engine->isReady())
  {
    output = engine->getPixelData();
    if(engine->update((float)millis() / 1000.0))
    {
      LedVector* pixels = engine->getPixelData();
      if(pixels)
      {
        if(pixels->value)
        {
          QPainter painter(pixmap);
          for (int i = 0; i < PIXELCOUNT; i++)
          {
            uint8_t r = constrain(output->value[WallLightConfig::LED_R][i], 0.0, 1.0) * 255;
            uint8_t g = constrain(output->value[WallLightConfig::LED_G][i], 0.0, 1.0) * 255;
            uint8_t b = constrain(output->value[WallLightConfig::LED_B][i], 0.0, 1.0) * 255;
            uint8_t ww = constrain(output->value[WallLightConfig::LED_WW][i], 0.0, 1.0) * 255;
            uint8_t cw = constrain(output->value[WallLightConfig::LED_CW][i], 0.0, 1.0) * 255;
            uint8_t am = constrain(output->value[WallLightConfig::LED_AM][i], 0.0, 1.0) * 255;
            painter.fillRect(offset, offset + hPixel * (PIXELCOUNT - 1 - i), 20, hPixel, QColor(r, g, b));
            painter.fillRect(offset + 20 + PIXELS_WWA_OFFSET, offset + hPixel * (PIXELCOUNT - 1 - i), 20, hPixel, QColor(ww, cw, am));
          }
          painter.end();
        }
      }
      else
      {
        console.error.println("[WALLLIGHT_TEST] Error occured while updating graph");
        engine->unloadGraph();
      }
    }
    else
    {
      QPainter painter(pixmap);
      painter.fillRect(offset, offset + hPixel * (PIXELCOUNT - 1), 20, hPixel, QColor(0, 0, 0));
      painter.end();
    }
  }
  
  label->setPixmap(*pixmap);
  label->move(0, 0);
  label->show();
  ui->centralWidget->update();
}

bool WallLightTest::loadGraph(const char* name)
{
  console.log.printf("[WALLLIGHT_TEST] Loading Module test ""%s""\n", name);
  testEngine->loadtest(name);
  return true;
}

void WallLightTest::unloadGraph()
{
  if(!engine->isReady())    // Check if graph is already unloaded
  {
    return;
  }
  console.log.println("[WALLLIGHT_TEST] Unloading graph from Engine pending...");
  unloadingGraphPending = true;
}
