#include "MQTT_EMQX.h"
#include <ArduinoJson.h>


// 初始化静态成员
Mqtt_emqx* Mqtt_emqx::instance = nullptr;

Mqtt_emqx::Mqtt_emqx(const char *ssid, const char *password, const char *mqtt_broker, const char *topic, const char *mqtt_username, const char *mqtt_password, const int mqtt_port, const char *topic_pub)
    : ssid(ssid), password(password), mqtt_broker(mqtt_broker), topic(topic), mqtt_username(mqtt_username), mqtt_password(mqtt_password), mqtt_port(mqtt_port), client(espClient), incomingMessage(""), messagePending(false), topic_pub(topic_pub) {
    instance = this;  // 在构造函数中设置全局实例指针
}//把构造函数的参数传递给类的私有成员变量，括号前的是类的私有成员变量，括号内的是构造函数的参数

void Mqtt_emqx::callback(char *topic, byte *payload, unsigned int length) {
    if (instance) {
        instance->incomingMessage = "";
        for (unsigned int i = 0; i < length; i++) {
            instance->incomingMessage += (char)payload[i];
        }
        instance->messagePending = true;        // 设置消息标志，可以开始执行消息处理
    }
}

bool Mqtt_emqx::connect() {
    // 连接WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");
    Serial.print("IPv4: ");
    Serial.println(WiFi.localIP());
    Serial.print("IPv6: ");
    Serial.println(WiFi.localIPv6());

    // 连接MQTT
    client.setServer(mqtt_broker, mqtt_port);
    client.setCallback(Mqtt_emqx::callback);

    while (!client.connected()) {
        String client_id = "esp32-client-";
        client_id += String(WiFi.macAddress());

        Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
        if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
            Serial.println("Public emqx mqtt broker connected");
        } else {
            Serial.print("failed with state ");
            Serial.print(client.state());
            delay(2000);
        }
    }

    client.subscribe(topic);
    return true;
}

void Mqtt_emqx::subscribe(const char *topic) {
    client.subscribe(topic);
}

void Mqtt_emqx::publish(const char *topic, const char *data) {
    client.publish(topic, data);
}




void Mqtt_emqx::loop() {
    client.loop();

}

