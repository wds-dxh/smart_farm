/*
 * @Author: wds-dxh wdsnpshy@163.com
 * @Date: 2024-07-12 17:08:47
 * @LastEditors: wds-dxh wdsnpshy@163.com
 * @LastEditTime: 2024-07-18 15:05:40
 * @FilePath: \esp32wroom_mqtt\src\LightSensorModbusRTU.cpp
 * @Description: 
 * 微信: 15310638214 
 * 邮箱：wdsnpshy@163.com 
 * Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
 */

#include "LightSensorModbusRTU.h"

// 构造函数，初始化串口引脚和波特率
LightSensorModbusRTU::LightSensorModbusRTU(uint8_t rxPin, uint8_t txPin, uint32_t baudRate)
    : _serial(2), _baudRate(baudRate) {
    _serial.begin(_baudRate, SERIAL_8N1, rxPin, txPin);
}

// 读取光照强度
bool LightSensorModbusRTU::readLightIntensity(uint8_t deviceAddress, float &lightIntensity) {
    // Modbus RTU请求帧格式
    uint8_t request[] = {
        deviceAddress, // 设备地址
        0x03,          // 功能码：读保持寄存器
        0x00, 0x02,    // 起始地址（高字节，低字节）
        0x00, 0x02     // 读取寄存器数量（高字节，低字节）
    };

    // 计算并附加CRC
    appendCRC(request, 6);

    // 发送请求帧
    _serial.write(request, 8);
    
    delay(500); // 等待传感器响应

    // 接收响应帧
    uint8_t response[9];
    int bytesRead = _serial.readBytes(response, 9);

    // 检查接收到的字节数是否正确
    if (bytesRead != 9) {
        return false; // 错误：未收到预期数量的字节
    }

    // 验证响应帧的CRC
    if (!verifyCRC(response, 7)) {
        return false; // 错误：CRC校验失败
    }

    // 解析响应帧
    if (response[1] != 0x03 || response[2] != 0x04) {
        return false; // 错误：意外的功能码或字节数
    }

    // 获取光照强度数据
    uint16_t high = (response[3] << 8) | response[4]; // 高16位
    uint16_t low = (response[5] << 8) | response[6];  // 低16位
    uint32_t lightIntensityRaw = (high << 16) | low;  // 合并高低16位数据

    // 转换为Lux
    lightIntensity = lightIntensityRaw / 1000.0; // 将32位光照数据除以1000，得到实际的Lux值
    return true; // 成功读取光照强度
}

// 计算并附加CRC
void LightSensorModbusRTU::appendCRC(uint8_t *frame, uint8_t length) {
    uint16_t crc = calculateCRC(frame, length);
    frame[length] = crc & 0xFF;        // CRC低字节
    frame[length + 1] = (crc >> 8) & 0xFF; // CRC高字节
}

// 验证CRC
bool LightSensorModbusRTU::verifyCRC(uint8_t *frame, uint8_t length) {
    uint16_t receivedCrc = (frame[length + 1] << 8) | frame[length];
    return calculateCRC(frame, length) == receivedCrc;
}

// 计算CRC16
uint16_t LightSensorModbusRTU::calculateCRC(uint8_t *frame, uint8_t length) {
    uint16_t crc = 0xFFFF; // 初始值为0xFFFF
    for (uint8_t pos = 0; pos < length; pos++) {
        crc ^= (uint16_t)frame[pos]; // 异或数据
        for (uint8_t i = 8; i != 0; i--) { // 处理8位数据
            if ((crc & 0x0001) != 0) { // 如果最低位为1
                crc >>= 1; // 右移一位
                crc ^= 0xA001; // 异或多项式
            } else {
                crc >>= 1; // 仅右移一位
            }
        }
    }
    return crc; // 返回计算得到的CRC值
}
