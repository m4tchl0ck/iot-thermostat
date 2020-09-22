#ifndef TemperatureSensor_h
#define TemperatureSensor_h

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Time.h>
#include <functional>

extern "C" {
  #include "user_interface.h"
}

class TemperatureSensor
{
  public:
    TemperatureSensor(byte pin, int readDelay);
    ~TemperatureSensor();
    void loop();

    void onTemperatureChanged(std::function<void(TemperatureSensor* temperatureSensor)> onTemperatureChanged);
    float Temperature;
    time_t SuccessReadTime;
  private:
    DHT_Unified* _sensor;
    long _readDelay;
    time_t _lastReadTime;
    std::function<void(TemperatureSensor* temperatureSensor)> _onTemperatureChanged;
};

#endif