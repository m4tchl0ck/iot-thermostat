#ifndef CONFIG_h
#define CONFIG_h

#include <ESP_EEPROM.h>

const byte SSID_LEN = 21;
const byte WIFI_PASSWD_LEN = 21;

struct ConfigurationStruct {
  int magicNo;
  float requiredTemperature;
  bool isActive;
  char ssid[SSID_LEN];
  char wifiPassword[WIFI_PASSWD_LEN];
};

class Config
{
private:
    ConfigurationStruct config;
    const int magicNo = 512730649;

    std::function<void(Config* config)> _onConfigChanged;
public:
    Config(/* args */);
    ~Config();
    bool save();
    float requiredTemperature();
    void requiredTemperature(float requiredTemperature);
    String ssid();
    void ssid(String ssid);
    boolean isActive();
    void isActive(boolean isActive);
    String wifiPassword();
    void wifiPassword(String wifiPassword);

    void onConfigChanged(std::function<void(Config* config)> onConfigChanged);
};

Config::Config(/* args */)
{
    EEPROM.begin(sizeof(ConfigurationStruct));
    EEPROM.get(0, config);
    if (config.magicNo != magicNo)
    {
        Serial.println("init config");
        ssid("Thermostat");
        isActive(false);
        save();
    }

    Serial.print("magicNo:"); Serial.println(magicNo);
    Serial.print("requiredTemperature:"); Serial.println(requiredTemperature());
    Serial.print("isActive:"); Serial.println(isActive());
    Serial.print("ssid:"); Serial.println(ssid());
}

Config::~Config()
{
}

bool Config::save()
{
    //check if any changed
    config.magicNo = magicNo;
    EEPROM.put(0, config);
    boolean ok = EEPROM.commit();

    if (_onConfigChanged && ok) {
        _onConfigChanged(this);
    }
    Serial.println((ok) ? "Commit OK" : "Commit failed");

    return ok;
}

void Config::onConfigChanged(std::function<void(Config* config)> onConfigChanged)
{
    _onConfigChanged = onConfigChanged;
}

float Config::requiredTemperature()
{
    return config.requiredTemperature;
}
void Config::requiredTemperature(float requiredTemperature)
{
    config.requiredTemperature = requiredTemperature;
}

String Config::ssid()
{
    String ssid = "";
    for (int i = 0; i <= SSID_LEN && config.ssid[i] != 0; i++)
    {
        ssid += (char)config.ssid[i];
    }
    return ssid;
}

void Config::ssid(String ssid)
{
    for (uint i = 0; i < ssid.length() && i <= SSID_LEN; i++)
    {
        config.ssid[i] = ssid.charAt(i);
    }
    config.ssid[ssid.length()] = 0;
}

String Config::wifiPassword()
{
    String wifiPassword = "";
    for (int i = 0; i <= WIFI_PASSWD_LEN && config.wifiPassword[i] != 0; i++)
    {
        wifiPassword += (char)config.wifiPassword[i];
    }
    return wifiPassword;
}

void Config::wifiPassword(String wifiPassword)
{
    for (uint i = 0; i < wifiPassword.length() && i <= WIFI_PASSWD_LEN; i++)
    {
        config.wifiPassword[i] = wifiPassword.charAt(i);
    }
    config.wifiPassword[wifiPassword.length()] = 0;
}

boolean Config::isActive() {
    return config.isActive;
}
void Config::isActive(boolean isActive) {
    config.isActive = isActive;
}

#endif