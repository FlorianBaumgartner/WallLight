#include "WallLightTest.h"
#include "ui_WallLightTest.h"

WallLightTest::WallLightTest(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WallLightTest)
{
    ui->setupUi(this);
}

WallLightTest::~WallLightTest()
{
    delete ui;
}

void WallLightTest::on_actionExit_triggered()
{
    this->close();
}
