'''
Author: wds-dxh wdsnpshy@163.com
Date: 2024-07-11 08:36:46
LastEditors: wds-dxh wdsnpshy@163.com
LastEditTime: 2024-07-17 16:31:29
FilePath: \后端\connext_esp32.py
Description: 
微信: 15310638214 
邮箱：wdsnpshy@163.com 
Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
'''

import paho.mqtt.client as mqtt
import json
class MQTTClient:
    def __init__(self, host, port, username, password, pub_topic, sub_topic):
        self.host = host
        self.port = port
        self.username = username
        self.password = password
        self.pub_topic = pub_topic
        self.sub_topic = sub_topic
        self.client = mqtt.Client()
        self.client.on_connect = self.on_connect
        self.client.on_message = self.on_message
    def connect(self):
        self.client.username_pw_set(self.username, self.password)
        self.client.connect(self.host, self.port, 60)
    def publish_message(self, position):
        message = json.dumps(position)
        self.client.publish(self.pub_topic, message, qos=1)
    def on_connect(self, client, userdata, flags, rc):
        print('Connected with result code ' + str(rc))
        self.client.subscribe(self.sub_topic)
    def on_message(self, client, userdata, msg):
        print(msg.topic + " " + str(msg.payload))
    def start(self):
        self.connect()
        self.client.loop_forever()
if __name__ == '__main__':
    mqtt_host = 'vps.dxh-wds.top'
    mqtt_port = 1883
    mqtt_pub_topic = 'agv_control_sub_topic'
    mqtt_username = 'wds'
    mqtt_password = 'wdsshy0320'
    mqtt_sub_topic = 'agv_esp32_pub_topic'
    mqtt_client = MQTTClient(mqtt_host, mqtt_port, mqtt_username, mqtt_password, mqtt_pub_topic, mqtt_sub_topic)
    mqtt_client.publish_message("2")  # Publish position 2
    mqtt_client.start()