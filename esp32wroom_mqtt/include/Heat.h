/*
 * @Author: wds-dxh wdsnpshy@163.com
 * @Date: 2024-07-12 16:58:58
 * @LastEditors: wds-dxh wdsnpshy@163.com
 * @LastEditTime: 2024-07-16 21:36:14
 * @FilePath: \esp32_mqtt\include\heat.h
 * @Description: 温度控制头文件
 * 微信: 15310638214 
 * 邮箱：wdsnpshy@163.com 
 * Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
 */
#ifndef __HEAT_H__
#define __HEAT_H__
#include <Arduino.h>

// Move the class declaration outside of the return type
class Heat{
    private:
        int pin;
    public:
        //构造函数
        Heat(int pin);
        //析构函数
        ~Heat();
        //开启加热
        void start();
        //关闭加热
        void stop();
};



#endif // __HEAT_H__