#ifndef HEATER_h
#define HEATER_h

class Heater
{
private:
    byte _pin;
    std::function<void(Heater* heater)> _onStateChanged;

public:
    Heater(byte pin);
    ~Heater();
    void turnOn();
    void turnOff();
    boolean getState();
    void onStateChanged(std::function<void(Heater* heater)> onStateChanged);
};

Heater::Heater(byte pin)
{
    _pin = pin;
    pinMode(_pin, OUTPUT);
}

Heater::~Heater()
{
}

void Heater::turnOn()
{
    digitalWrite(_pin, HIGH);

    if (_onStateChanged) {
        _onStateChanged(this);
    }
}

void Heater::turnOff()
{
    digitalWrite(_pin, LOW);

    if (_onStateChanged) {
        _onStateChanged(this);
    }
}

boolean Heater::getState()
{
    return digitalRead(_pin) == HIGH;
}

void Heater::onStateChanged(std::function<void(Heater* heater)> onStateChanged)
{
    _onStateChanged = onStateChanged;
}

#endif