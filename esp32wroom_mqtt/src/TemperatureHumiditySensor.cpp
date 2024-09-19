
/*
 * @Author: wds-dxh wdsnpshy@163.com
 * @Date: 2024-07-09 08:26:13
 * @LastEditors: wds-dxh wdsnpshy@163.com
 * @LastEditTime: 2024-07-18 18:45:27
 * @FilePath: \esp32wroom_mqtt\src\TemperatureHumiditySensor.cpp
 * @Description: 
 * 微信: 15310638214 
 * 邮箱：wdsnpshy@163.com 
 * Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
 */
#include "TemperatureHumiditySensor.h"

TemperatureHumiditySensor::TemperatureHumiditySensor(uint8_t rxPin, uint8_t txPin, uint32_t baudRate) 
    : _serial(1) {  
    _serial.begin(baudRate, SERIAL_8N1, rxPin, txPin);
}

bool TemperatureHumiditySensor::readTemperatureHumidity(uint8_t deviceAddress, float &temperature, float &humidity) {
    uint8_t requestFrame[8] = {deviceAddress, 0x03, 0x00, 0x00, 0x00, 0x02};
    appendCRC(requestFrame, 6);     // 添加CRC校验码

    _serial.write(requestFrame, 8);
    delay(500); // Wait for the response

    // Serial.write(requestFrame, 8);

    if (_serial.available() >= 9) {
        uint8_t responseFrame[9];
        _serial.readBytes(responseFrame, 9);

        //打印接受到的数据
        // Serial.write(responseFrame, 9);

        if (verifyCRC(responseFrame, 7)) {
            humidity = ((responseFrame[3] << 8) | responseFrame[4]) / 10.0;
            int16_t tempRaw = (responseFrame[5] << 8) | responseFrame[6];
            temperature = tempRaw / 10.0;
            return true;
        }
    }

    return false;
}

void TemperatureHumiditySensor::appendCRC(uint8_t *frame, uint8_t length) {
    uint16_t crc = calculateCRC(frame, length);     // 计算CRC校验码
    frame[length] = crc & 0xFF;
    frame[length + 1] = (crc >> 8) & 0xFF;
}

bool TemperatureHumiditySensor::verifyCRC(uint8_t *frame, uint8_t length) {
    uint16_t receivedCRC = (frame[length + 1] << 8) | frame[length];
    return receivedCRC == calculateCRC(frame, length);
}

uint16_t TemperatureHumiditySensor::calculateCRC(uint8_t *frame, uint8_t length) {
    uint16_t crc = 0xFFFF;
    for (uint8_t pos = 0; pos < length; pos++) {
        crc ^= (uint16_t)frame[pos];
        for (uint8_t i = 8; i != 0; i--) {
            if ((crc & 0x0001) != 0) {
                crc >>= 1;
                crc ^= 0xA001;      // 0xA001是CRC16的多项式
            } else {    
                crc >>= 1;
            }
        }
    }
    return crc;
}
