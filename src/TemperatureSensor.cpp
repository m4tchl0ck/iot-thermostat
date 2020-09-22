#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(byte pin, int readDelay)
{
  _sensor = new DHT_Unified(pin, DHT22);
  _sensor->begin();

  sensor_t sensorInfo;
  _sensor->temperature().getSensor(&sensorInfo);
  long minDelay =  sensorInfo.min_delay / 1000 / 1000;
  _readDelay = minDelay > readDelay ? minDelay : readDelay;
  _lastReadTime = time(nullptr);
}

void TemperatureSensor::loop()
{
  time_t now = time(nullptr);
  if (now - _lastReadTime < _readDelay)
  {
    return;
  }
  Serial.printf("last: %ld\n", _lastReadTime);
  Serial.printf("now: %ld\n", now);
  Serial.printf("delay: %ld\n", _readDelay);
  sensors_event_t event;
  _sensor->temperature().getEvent(&event);
  _lastReadTime = time(nullptr);
  if (isnan(event.temperature))
  {
    Serial.println(F("Error reading temperature!"));
  }
  else
  {
    float currentTemp = event.temperature;
    if (currentTemp != Temperature)
    {
      Temperature = event.temperature;
      if (_onTemperatureChanged)
      {
        _onTemperatureChanged(this);
      }
    }
    SuccessReadTime = now;
  }
}

void TemperatureSensor::onTemperatureChanged(std::function<void(TemperatureSensor* temperatureSensor)> onTemperatureChanged)
{
  _onTemperatureChanged = onTemperatureChanged;
}