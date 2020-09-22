#ifndef CONFIGAPI_h
#define CONFIGAPI_h

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "Config.h"

class ConfigApi
{
private:
    ESP8266WebServer* _httpServer;
    Config* _config;
    void handleGet();
    void handlePatch();
public:
    ConfigApi(ESP8266WebServer* httpServer, Config* config);
    ~ConfigApi();
};

ConfigApi::ConfigApi(ESP8266WebServer* httpServer, Config* config)
{
    _httpServer = httpServer;
    _config = config;

    _httpServer->on(String(F("/api/config")), HTTP_GET, std::bind(&ConfigApi::handleGet, this));
    _httpServer->on(String(F("/api/config")), HTTP_PATCH, std::bind(&ConfigApi::handlePatch, this));

}

ConfigApi::~ConfigApi()
{
}

void ConfigApi::handleGet()
{
  StaticJsonDocument<512> doc;
  doc["ssid"] = _config->ssid();
  doc["pass"] = _config->wifiPassword();
  String result;
  serializeJson(doc, result);

  _httpServer->send(200, "application/json", result);
}

void ConfigApi::handlePatch()
{
  Serial.println("plain: " + _httpServer->arg("plain"));
  
  StaticJsonDocument<512> doc;
  deserializeJson(doc, _httpServer->arg("plain"));

  if (doc["ssid"].is<String>()) {
    _config->ssid(doc["ssid"].as<String>());
    Serial.println("set: ssid: " + doc["ssid"].as<String>());
  }
  if (doc["pass"].is<String>()) {
    _config->wifiPassword(doc["pass"].as<String>());
    Serial.println("set: pass: " + doc["pass"].as<String>());
  }

  if (_config->save()) {
    _httpServer->send(202, "text/plain", "");
  } else
  {
    _httpServer->send(503, "text/plain", "");
  }
}

#endif