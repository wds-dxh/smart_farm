#ifndef TemperatureHumiditySensor_h
#define TemperatureHumiditySensor_h

#include <Arduino.h>
#include <HardwareSerial.h>

class TemperatureHumiditySensor {
public:
    TemperatureHumiditySensor(uint8_t rxPin, uint8_t txPin, uint32_t baudRate = 4800);

    bool readTemperatureHumidity(uint8_t deviceAddress, float &temperature, float &humidity);

private:
    HardwareSerial _serial;
    void appendCRC(uint8_t *frame, uint8_t length);
    bool verifyCRC(uint8_t *frame, uint8_t length);
    uint16_t calculateCRC(uint8_t *frame, uint8_t length);
};

#endif
