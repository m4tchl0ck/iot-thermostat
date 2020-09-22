#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

#include "TemperatureSensor.h"
#include "Config.h"
#include "Heater.h"
#include "Thermostat.h"

#include "UI.h"
#include "ThermostatApi.h"
#include "ConfigApi.h"

ESP8266WebServer *httpServer;
DNSServer *dnsServer;

TemperatureSensor *temperatureSensor;
Config *config;
Heater *heater;
Thermostat *thermostat;

UI *ui;
ThermostatApi *thermostatApi;
ConfigApi *configApi;

void changeHeaterState(TemperatureSensor *temperatureSensor1);
void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("setup");

  config = new Config();
  temperatureSensor = new TemperatureSensor(D4, 10 * 60);
  thermostat = new Thermostat(config, new Heater(D1), temperatureSensor);

  httpServer = new ESP8266WebServer(80);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(config->ssid(), config->wifiPassword());

  Serial.println("ssid: " + config->ssid());
  Serial.println("pass: " + config->wifiPassword());
  dnsServer = new DNSServer();
  dnsServer->setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer->start(53, "*", WiFi.softAPIP());

  ui = new UI(httpServer);
  thermostatApi = new ThermostatApi(httpServer, thermostat);
  configApi = new ConfigApi(httpServer, config);

  httpServer->begin();
}

void loop()
{
  dnsServer->processNextRequest();
  httpServer->handleClient();

  temperatureSensor->loop();
}
