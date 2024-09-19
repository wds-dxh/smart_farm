/*
 * @Author: wds-dxh wdsnpshy@163.com
 * @Date: 2024-07-08 10:18:57
 * @LastEditors: wds-dxh wdsnpshy@163.com
 * @LastEditTime: 2024-07-18 18:46:41
 * @FilePath: \esp32wroom_mqtt\src\main.cpp
 * @Description: 
 * 微信: 15310638214 
 * 邮箱：wdsnpshy@163.com 
 * Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
 */
#include <soc/soc.h> 
#include <soc/rtc_cntl_reg.h>  //关闭低电压检测,避免无限重启

#include <Arduino.h>
#include "MQTT_EMQX.h"
#include "TemperatureHumiditySensor.h"
#include <ArduinoJson.h>
#include <LightSensorModbusRTU.h>  // 光照传感器头文件
#include <Water_pump.h>  // 水泵头文件
#include "Heat.h"   // 加热头文件
#include <lamp.h>


// WiFi和MQTT参数
const char *ssid = "wds";
const char *password = "wds666666";
const char *mqtt_broker = "vps.dxh-wds.top";
const char *topic = "control_smart_farm";
const char *mqtt_username = "wds";
const char *mqtt_password = "wdsshy0320";
const int mqtt_port = 1883;
const char *topic_pub = "smart_farm_status";

Mqtt_emqx mqtt(ssid, password, mqtt_broker, topic, mqtt_username, mqtt_password, mqtt_port, topic_pub);//实例化mqtt对象
TemperatureHumiditySensor sensor(1,2,4800);  // 设置传感器的RX和TX引脚,串口1
LightSensorModbusRTU lightSensorModbusRTU(41,42,9600);    //串口2

Heat heat(40);  // 设置加热器引脚
Water_pump water_pump(39);  // 设置水泵引脚
RGBColor color = {255,255,255};//定义一个颜色结构体
Lamp lamp(255,10,38,color);//定义一个lamp对象0    初始颜色为白灯

void Xcontrol_wifi(void *pvParameters);
void Xothers(void *pvParameters); 

void setup() {
  
    Serial.begin(115200);// 初始化串口  默认引脚44 43 rx tx
    Serial.println("Hello, ESP32!");
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);//关闭低电压检测,避免无限重启
    
    mqtt.connect();// 连接WiFi和MQTT
    mqtt.publish(topic_pub, "esp32 goes live");
    
    heat.stop();  // 关闭加热器
    water_pump.stop();  // 关闭水泵
    lamp.lamp_white();

    xTaskCreatePinnedToCore(Xcontrol_wifi, "TaskOne", 15000, NULL, 2, NULL, 0);  //Xcontrol_wifi在 0核心
    xTaskCreatePinnedToCore(Xothers, "TaskTwo", 4096, NULL, 1, NULL, 1);  //Xothers在 1核心

}

float temperature, humidity;
float lightIntensity;
void loop() {

  mqtt.loop();  // 处理 MQTT 客户端循环

}


void Xcontrol_wifi(void *pvParameters){
  while (1)
  {

    // 读取温湿度传感器
    sensor.readTemperatureHumidity(0x01, temperature, humidity);
    // 读取光照传感器
    delay(1000);
    lightSensorModbusRTU.readLightIntensity(0x01, lightIntensity);
    delay(1000);

     // 构建 JSON 格式的消息
    StaticJsonDocument<100> doc;  // 适当调整大小以适应你的数据
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["illumination"] = lightIntensity;
    char jsonMessage[100];  // 定义用于存放 JSON 字符串的缓冲区
    serializeJson(doc, jsonMessage, sizeof(jsonMessage));  // 序列化 JSON 对象为字符串
    mqtt.publish(topic_pub, jsonMessage);  // 发布 JSON 格式的消息到 MQTT 主题 topic_pub

  
  }

   vTaskDelete(NULL);  

}
 




void Xothers(void *pvParameters) 
{
  while (1){

    //处理MQTT客户端循环
    if (mqtt.messagePending) {
        mqtt.publish(topic_pub, mqtt.incomingMessage.c_str());  // 发布消息, 用于调试

        // 解析 JSON 消息
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, mqtt.incomingMessage);
        if (!error) {
            const char* Water_pump = doc["Water_pump"];
            const char* Heat = doc["Heat"];
            const char* luminance = doc["luminance"];

            if (strcmp(Water_pump, "on") == 0) {
                water_pump.start();
            } else if (strcmp(Water_pump, "off") == 0) {
                water_pump.stop();
            }
            if (strcmp(Heat, "on") == 0) {
                heat.start();
            } else if (strcmp(Heat, "off") == 0) {
                heat.stop();
            }
            int lamp_luminance_value = atoi(luminance);
            lamp.lamp_luminance(lamp_luminance_value);
            Serial.println(lamp_luminance_value);
        }

        mqtt.messagePending = false; // 重置消息标志
    }
    delay(1000);

  }
  

  vTaskDelete(NULL);

}