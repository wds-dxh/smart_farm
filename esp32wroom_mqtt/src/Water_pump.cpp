/*
 * @Author: wds-dxh wdsnpshy@163.com
 * @Date: 2024-07-20 13:49:52
 * @LastEditors: wds-dxh wdsnpshy@163.com
 * @LastEditTime: 2024-07-26 09:08:32
 * @FilePath: /esp32wroom_mqtt/src/Water_pump.cpp
 * @Description: 
 * 微信: 15310638214 
 * 邮箱：wdsnpshy@163.com 
 * Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
 */
#include "water_pump.h"

Water_pump::Water_pump(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

Water_pump::~Water_pump()
{
}

void Water_pump::start()
{

    digitalWrite(this->pin, HIGH);  // 开启水泵
}

void Water_pump::stop()
{
    digitalWrite(this->pin, LOW);   // 关闭水泵
}
