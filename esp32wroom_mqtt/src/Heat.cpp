/*
 * @Author: wds-dxh wdsnpshy@163.com
 * @Date: 2024-07-12 16:58:46
 * @LastEditors: wds-dxh wdsnpshy@163.com
 * @LastEditTime: 2024-07-12 17:04:05
 * @FilePath: \esp32_mqtt\src\heat.cpp
 * @Description: 温度控制
 * 微信: 15310638214 
 * 邮箱：wdsnpshy@163.com 
 * Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
 */
#include "heat.h"


/**
 * @Author: wds-dxh wdsnpshy@163.com
 * @Date: 2024-07-12 17:04:21
 * @description: 初始化温度控制
 * @param {int} pin
 * @return {*}
 */
Heat::Heat(int pin)
{
    this->pin = pin;
    pinMode(pin, OUTPUT);
}

Heat::~Heat()
{
    // do nothing
}

void Heat::start()
{
    digitalWrite(this->pin, LOW);  // 开启加热
}

void Heat::stop()
{
    digitalWrite(this->pin, HIGH);   // 关闭加热
}