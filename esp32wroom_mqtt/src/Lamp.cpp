/*
 * @Author: wds-dxh wdsnpshy@163.com
 * @Date: 2024-07-17 17:37:23
 * @LastEditors: wds-dxh wdsnpshy@163.com
 * @LastEditTime: 2024-07-17 20:46:47
 * @FilePath: \esp32wroom_mqtt\src\Lamp.cpp
 * @Description: 
 * 微信: 15310638214 
 * 邮箱：wdsnpshy@163.com 
 * Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
 */
/**
 * @file 灯光控制（ws2812b）
 * @author wds
 * @date 2023/10/8
 * 邮箱：wdsnpshy@163.com
 * 博客：https://blog.csdn.net/weixin_63211230
 * qq:3412363587
 */

#include "Lamp.h"

/**
 * @brief 构造函数
 * @param luminance_int 亮度
 * @param LED_COUNT_int 灯珠数量
 * @param LED_PIN_int   控制引脚
 * @param color_int     颜色
 * @return 无
*/
Lamp :: Lamp(uint8_t luminance_int,uint16_t LED_COUNT_int, int16_t LED_PIN_int,RGBColor color_int)
    : strip(LED_COUNT_int, LED_PIN_int, NEO_GRB + NEO_KHZ800)      {     //定义灯光对象 
        luminance = luminance_int; //将传入的亮度赋值给类的亮度，方便亮度管理
        LED_COUNT = LED_COUNT_int; //将传入的灯珠数量赋值给类的灯珠数量，方便灯珠数量管理
        LED_PIN = LED_PIN_int;     //将传入的控制引脚赋值给类的控制引脚，方便控制引脚管理
        color = color_int; //将传入的颜色赋值给类的颜色，方便颜色管理

        strip.begin();  //初始化灯光
        // strip.show();
        strip.setBrightness(luminance);  //设置灯光亮度

}


/**
 * @brief 控制全部灯光
 * @param color_transfer 颜色
 * @return 无
*/
void Lamp :: lamp_color(RGBColor color_transfer){
    // Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, color_transfer.red, color_transfer.green, color_transfer.blue); //设置灯光颜色
    strip.show();
  }
  

}


/**
 * @brief 控制亮度
 * @param luminance_transfer 亮度
 * @return 无
*/
void Lamp :: lamp_luminance(uint8_t luminance_transfer){
    // Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
    for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, luminance_transfer, luminance_transfer, luminance_transfer); //设置灯光颜色
    strip.show();
      }
}



/**
 * @brief 控制所有灯珠为白色
 * @param 无
 * @return 无
*/
void Lamp :: lamp_white(){
    // Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// for (int i = 67; i < 77; i++) {
for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, 255, 255, 255); //设置灯光颜色
    strip.show();
      }
      
}

/**
 * @brief 控制所有灯珠为黑色
 * @param 无
 * @return 无
*/
void Lamp :: lamp_off(){
    // Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
    for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, 0, 0, 0); //设置灯光颜色
    strip.show();
      }
       
}

