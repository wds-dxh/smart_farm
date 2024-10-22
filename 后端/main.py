import json
import pymysql
from flask import Flask, request, jsonify
from flask_cors import CORS  # pip install flask-cors 
import threading
from connext_esp32 import MQTTClient

app = Flask(__name__)
CORS(app)  # 允许所有域名的所有请求

# MQTT配置
mqtt_host = 'vps.dxh-wds.top'
mqtt_port = 1883
mqtt_pub_topic = 'control_smart_farm'  # 控制智能农场
mqtt_username = 'wds'
mqtt_password = 'wdsshy0320'
mqtt_sub_topic = 'test'

# 创建MQTT客户端
mqtt_client = MQTTClient(mqtt_host, mqtt_port, mqtt_username, mqtt_password, mqtt_pub_topic, mqtt_sub_topic)

# 修改MQTT客户端的on_message回调
def on_message(client, userdata, msg):
    pass

mqtt_client.client.on_message = on_message

# 启动MQTT客户端
def start_mqtt_client():
    mqtt_client.start()

# 在单独的线程中启动MQTT客户端
mqtt_thread = threading.Thread(target=start_mqtt_client)
mqtt_thread.start()


# 控制智能农场设备的API
@app.route('/smart_farm/control', methods=['POST'])
def control_smart_farm():
    try:
        control_data = request.json
        if not control_data:
            return jsonify({'error': 'No control data received'}), 400
        
        # MQTT消息必须保持JSON格式
        mqtt_message = {
            "Water_pump": control_data.get("Water_pump", "off"),
            "Heat": control_data.get("heat", "off"),
            "luminance": control_data.get("luminance", "0")
        }
        
        # 将字典转换成JSON字符串发送
        mqtt_client.client.publish(mqtt_pub_topic, json.dumps(mqtt_message))
        
        return jsonify({'message': 'Control data sent successfully via MQTT'}), 200
    except Exception as e:
        print(f"Error in control_smart_farm: {str(e)}")
        return jsonify({'error': 'Failed to send control data'}), 500


@app.route('/api/login', methods=['POST'])
def login():
    data = request.get_json()
    username = data.get('username')
    password = data.get('password')

    con = get_db_connection()
    if not con:
        return jsonify({"error": "数据库连接失败"}), 500

    try:
        with con.cursor() as cursor:
            sql = "SELECT * FROM smart_farm_login WHERE username=%s AND password=%s"
            cursor.execute(sql, (username, password))
            result = cursor.fetchone()

        if result:
            return jsonify(success=True, redirect_url="/Dashboard")
        else:
            return jsonify(success=False), 401
    except Exception as e:
        print("查询失败")
        print(e)
        return jsonify({"error": "查询失败"}), 500
    finally:
        con.close()

@app.route('/smart_farm/status', methods=['POST'])
def get_workshop_status():
    try:
        data = request.get_json()
        limit = data.get('limit', 10)  # 默认查询最近的 10 条记录

        # 连接数据库
        con = get_db_connection()
        if not con:
            return jsonify({"error": "数据库连接失败"}), 500
        print("数据库连接成功")

        # 查询数据的 SQL 语句
        sql = f"""
        SELECT temperature, humidness, illumination, timestamp
        FROM smart_farm_status
        ORDER BY timestamp DESC
        LIMIT {limit}
        """
        # 使用游标对象执行 SQL 语句并获取结果
        with con.cursor() as cursor:
            cursor.execute(sql)
            result = cursor.fetchall()
            data = []
            for row in result:
                data.append({
                    "temperatures": row[0],
                    "humidnesses": row[1],
                    "illuminations": row[2],
                    "timestamps": row[3].strftime('%Y-%m-%d %H:%M:%S')
                })
            return jsonify(data)
    except Exception as e:
        print("数据库连接失败")
        print(e)
        return jsonify({"error": "Database connection failed"}), 500
    finally:
        if con:
            con.close()


#链接数据库
def get_db_connection():
    try:
        con = pymysql.connect(
            host='vps.dxh-wds.top',
            user='smartfarm',
            password='smartfarm',
            database='smart_farm',  # 指定数据库
        )
        return con
    except Exception as e:
        print("数据库连接失败")
        print(e)
        return None

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=True)
