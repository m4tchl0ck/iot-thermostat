#ifndef UI_h
#define UI_h

#include "FS.h"

class UI
{
private:
  ESP8266WebServer* _httpServer;
  void streamFile(String fileName, String contentType);

  void handleNotFound();
  boolean captivePortal();
  boolean isIp(String str);
  String toStringIp(IPAddress ip);
public:
  UI(ESP8266WebServer* httpServers);
  ~UI();
};

UI::UI(ESP8266WebServer* httpServer)
{
  _httpServer = httpServer;

  SPIFFS.begin();

  _httpServer->on("/", HTTP_GET, std::bind(&UI::streamFile, this, "/index.html", "text/html"));
  _httpServer->on("/app.js", HTTP_GET, std::bind(&UI::streamFile, this, "/app.js", "application/javascript"));

  _httpServer->onNotFound(std::bind(&UI::handleNotFound, this));
}

UI::~UI()
{
}

void UI::streamFile(String fileName, String contentType)
{
  File dataFile = SPIFFS.open(fileName, "r");
  if (_httpServer->streamFile(dataFile, contentType) != dataFile.size())
  {
      Serial.println("Sent less data than expected!");
  }
  dataFile.close();
}


void UI::handleNotFound() {
  if (captivePortal()) {
    return;
  }
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += _httpServer->uri();
  message += "\nMethod: ";
  message += ( _httpServer->method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += _httpServer->args();
  message += "\n";

  for ( uint8_t i = 0; i < _httpServer->args(); i++ ) {
    message += " " + _httpServer->argName ( i ) + ": " + _httpServer->arg ( i ) + "\n";
  }
  _httpServer->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  _httpServer->sendHeader("Pragma", "no-cache");
  _httpServer->sendHeader("Expires", "-1");
  _httpServer->sendHeader("Content-Length", String(message.length()));
  _httpServer->send ( 404, "text/plain", message );
}


boolean UI::captivePortal() {
  if (isIp(_httpServer->hostHeader()) ) {
    return false;
  }
  _httpServer->sendHeader("Location", String("http://") + toStringIp(_httpServer->client().localIP()) + String("/"), true);
  _httpServer->send ( 302, "text/plain", "");
  _httpServer->client().stop();
  return true;
}

boolean UI::isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}

String UI::toStringIp(IPAddress ip) {
  String res = "";
  for (int i = 0; i < 3; i++) {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
#endif