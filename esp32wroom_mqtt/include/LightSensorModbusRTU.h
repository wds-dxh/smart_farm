#ifndef LIGHT_SENSOR_MODBUS_RTU_H
#define LIGHT_SENSOR_MODBUS_RTU_H

#include <Arduino.h>
#include <HardwareSerial.h>

class LightSensorModbusRTU {
public:
    // 构造函数，初始化串口引脚和波特率
    LightSensorModbusRTU(uint8_t rxPin, uint8_t txPin, uint32_t baudRate = 9600);

    // 读取光照强度
    bool readLightIntensity(uint8_t deviceAddress, float &lightIntensity);

private:
    HardwareSerial _serial;
    uint32_t _baudRate;

    // CRC校验相关函数
    void appendCRC(uint8_t *frame, uint8_t length);
    bool verifyCRC(uint8_t *frame, uint8_t length);
    uint16_t calculateCRC(uint8_t *frame, uint8_t length);
};

#endif // LIGHT_SENSOR_MODBUS_RTU_H
