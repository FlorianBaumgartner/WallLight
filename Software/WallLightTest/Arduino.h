#ifndef ARDUINO_H
#define ARDUINO_H

// #define _min(a,b) ((a)<(b)?(a):(b))  
// #define _max(a,b) ((a)>(b)?(a):(b))
#define _abs(x) ((x)>0?(x):-(x))  // abs() comes from STL
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define _round(x)     ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))  // round() comes from STL
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

#define boolean bool
#define SERIAL_8N1 0

#include "Arduino/Stream.h"

#include <QThread>
#include <QDateTime>


static void delay(int t)
{
  QThread::msleep(t);
}

static int millis(void)
{
  static qint64 t0 = QDateTime::currentMSecsSinceEpoch();
  return static_cast<int>(QDateTime::currentMSecsSinceEpoch() - t0);
}

typedef const char*  esp_event_base_t;

class USBCDC
{
  public:
    size_t write(uint8_t c)
    {
      return putchar(c);
    }

    operator bool() const {return true;}
};

class HardwareSerial : public USBCDC {};


// #define min _min
// #define max _max



// #define min(a,b) ((a)<(b)?(a):(b))  
// #define max(a,b) ((a)>(b)?(a):(b))


#include "math.h"
#include "Arduino/console.hpp"

using namespace std;

// #define max std::max
// #define min std::min

#endif