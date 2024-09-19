#ifndef WATER_PUMP_H
#define WATER_PUMP_H

#include <Arduino.h>  // 确保包含 Arduino 头文件

class Water_pump {
public:
    Water_pump(int pin);  // 构造函数
    ~Water_pump();        // 析构函数

    void start();         // 开始水泵
    void stop();          // 停止水泵

private:
    int pin;              // 引脚
};

#endif
