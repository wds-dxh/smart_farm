/*
 * @Author: wds-dxh wdsnpshy@163.com
 * @Date: 2024-07-08 16:41:10
 * @LastEditors: wds-dxh wdsnpshy@163.com
 * @LastEditTime: 2024-07-09 08:24:24
 * @FilePath: \esp32_mqtt\include\mqtt_emqx.h
 * @Description: 
 * 微信: 15310638214 
 * 邮箱：wdsnpshy@163.com 
 * Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
 */
#ifndef MQTT_EMQX_H
#define MQTT_EMQX_H

#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

class Mqtt_emqx {
    private:
        const char *ssid;
        const char *password;
        const char *mqtt_broker;
        const char *topic;
        const char *mqtt_username;
        const char *mqtt_password;
        const int mqtt_port;
        const char *topic_pub;
        
        WiFiClient espClient;
        PubSubClient client;

        static void callback(char *topic, byte *payload, unsigned int length);

    public:
        static Mqtt_emqx* instance;     // 全局实例指针
        String incomingMessage;
        bool messagePending;

        // 初始化连接参数
        Mqtt_emqx(const char *ssid, const char *password, const char *mqtt_broker, const char *topic, const char *mqtt_username, const char *mqtt_password, const int mqtt_port, const char *topic_pub);
        
        // 连接WiFi和MQTT
        bool connect();
        
        // 订阅主题
        void subscribe(const char *topic);
        
        // 发布消息
        void publish(const char *topic, const char *data);

        // 处理MQTT客户端循环
        void loop();
};

#endif // MQTT_EMQX_H
