#ifndef WALLLIGHT_TEST_H
#define WALLLIGHT_TEST_H

#include <QMainWindow>

namespace Ui {
class WallLightTest;
}

class WallLightTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit WallLightTest(QWidget *parent = 0);
    ~WallLightTest();

private slots:
    void on_actionExit_triggered();

private:
    Ui::WallLightTest *ui;
};

#endif // WALLLIGHT_TEST_H
