#ifndef THERMOSTATAPI_H
#define THERMOSTATAPI_H

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "Thermostat.h"

extern "C"
{
#include "user_interface.h"
}

class ThermostatApi
{
public:
  ThermostatApi(ESP8266WebServer *httpServer, Thermostat *thermostat);
  ~ThermostatApi();

private:
  ESP8266WebServer *_httpServer;
  Thermostat *_thermostat;
  void handleTemperatureGet();
  void handleRequiredTemperatureGet();
  void handleRequiredTemperaturePut();
  void handleHeaterGet();
  void handleHeaterPut();
  void handleStateGet();
  void handleStatePut();
};

ThermostatApi::ThermostatApi(ESP8266WebServer *httpServer, Thermostat *thermostat)
{
  _thermostat = thermostat;
  _httpServer = httpServer;

  _httpServer->on(String(F("/api/temperature")), HTTP_GET, std::bind(&ThermostatApi::handleTemperatureGet, this));
  _httpServer->on(String(F("/api/requiredTemperature")), HTTP_GET, std::bind(&ThermostatApi::handleRequiredTemperatureGet, this));
  _httpServer->on(String(F("/api/requiredTemperature")), HTTP_PUT, std::bind(&ThermostatApi::handleRequiredTemperaturePut, this));
  _httpServer->on(String(F("/api/heater")), HTTP_GET, std::bind(&ThermostatApi::handleHeaterGet, this));
  _httpServer->on(String(F("/api/heater")), HTTP_PUT, std::bind(&ThermostatApi::handleHeaterPut, this));
  _httpServer->on(String(F("/api/state")), HTTP_GET, std::bind(&ThermostatApi::handleStateGet, this));
  _httpServer->on(String(F("/api/state")), HTTP_PUT, std::bind(&ThermostatApi::handleStatePut, this));
}

void ThermostatApi::handleTemperatureGet()
{
  StaticJsonDocument<512> doc;
  doc["temperature"] = _thermostat->temperature();
  doc["isError"] = _thermostat->isErrorRead();
  String result;
  serializeJson(doc, result);

  _httpServer->send(200, "application/json", result);
}

void ThermostatApi::handleRequiredTemperatureGet() {
  StaticJsonDocument<512> doc;
  doc["requiredTemperature"] = _thermostat->requiredTemperature();
  String result;
  serializeJson(doc, result);

  _httpServer->send(200, "application/json", result);
}
void ThermostatApi::handleRequiredTemperaturePut() {
  float requiredTemperature = atof(_httpServer->arg("plain").c_str());
  _thermostat->requiredTemperature(requiredTemperature);
  _httpServer->send(202, "text/plain", "");
}

void ThermostatApi::handleStateGet() {
  String result = String(_thermostat->state());
  _httpServer->send(200, "text/plain", result);
}
void ThermostatApi::handleStatePut() {
  String state = _httpServer->arg("plain");
  if (state == "true")
  {
    _thermostat->activate();
    _httpServer->send(202, "text/plain", "");
  }
  else if (state == "false")
  {
    _thermostat->deactivate();
    _httpServer->send(202, "text/plain", "");
  }
  else
  {
    _httpServer->send(400, "text/plain", "");
  }
}

void ThermostatApi::handleHeaterGet()
{
  String result = String(_thermostat->heater());
  _httpServer->send(200, "text/plain", result);
}
void ThermostatApi::handleHeaterPut()
{
  String newState = _httpServer->arg("plain");
  Serial.println(newState);
  if (newState == "true")
  {
    _thermostat->heater(true);
    _httpServer->send(202, "text/plain", "");
  }
  else if (newState == "false")
  {
    _thermostat->heater(false);
    _httpServer->send(202, "text/plain", "");
  }
  else
  {
    _httpServer->send(400, "text/plain", "");
  }
}

ThermostatApi::~ThermostatApi() 
{

}

#endif