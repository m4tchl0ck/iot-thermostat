#ifndef THERMOSTAT_h
#define THERMOSTAT_h

#include "Config.h"
#include "Heater.h"
#include "TemperatureSensor.h"

class Thermostat
{
private:
    Config* _config;
    Heater* _heater;
    TemperatureSensor* _temperatureSensor;

    void temperatureChanged(TemperatureSensor *sensor);

public:
    Thermostat(Config *config, Heater *heater, TemperatureSensor *temperatureSensor);
    ~Thermostat();
    void heater(boolean state);
    boolean heater();
    float requiredTemperature();
    void requiredTemperature(float requiredTemperature);
    boolean state();
    void activate();
    void deactivate();
    float temperature();
    bool isErrorRead();
};

Thermostat::Thermostat(Config *config, Heater *heater, TemperatureSensor *temperatureSensor)
{
    _config = config;
    _temperatureSensor = temperatureSensor;
    _heater = heater;

    _temperatureSensor->onTemperatureChanged([&](TemperatureSensor* sensor) {
        temperatureChanged(sensor);
    });
}

float Thermostat::temperature()
{
    return _temperatureSensor->Temperature;
}
bool Thermostat::isErrorRead()
{
    return _temperatureSensor->SuccessReadTime == 0 || (_temperatureSensor->SuccessReadTime - time(nullptr)) > (5 * 60);
}
void Thermostat::temperatureChanged(TemperatureSensor *sensor)
{
    if (_config->isActive())
    {
        if (sensor->Temperature < _config->requiredTemperature() && !isErrorRead())
        {
            _heater->turnOn();
        }
        else
        {
            _heater->turnOff();
        }
    }
}

float Thermostat::requiredTemperature()
{ 
    return _config->requiredTemperature();
}
void Thermostat::requiredTemperature(float requiredTemperature)
{
    _config->requiredTemperature(requiredTemperature);
    _config->save();
}

boolean Thermostat::state()
{
    return _config->isActive();
}
void Thermostat::activate()
{
    _config->isActive(true);
    _config->save();
    temperatureChanged(_temperatureSensor);
}
void Thermostat::deactivate()
{
    _config->isActive(false);
    _config->save();
    _heater->turnOff();
}

void Thermostat::heater(boolean state)
{
    deactivate();
    if (state)
    {
        _heater->turnOn();
    }
    else
    {
        _heater->turnOff();
    }
}
boolean Thermostat::heater()
{
    return _heater->getState();
}

Thermostat::~Thermostat()
{
}

#endif