import paho.mqtt.client as mqtt
import json
from pymysql import Connection

# 连接数据库
try:
    # 连接数据库
    con = Connection(
        host='vps.dxh-wds.top',
        user='smartfarm',
        password='smartfarm',
        database='smart_farm',  # 指定数据库
    )
    print("数据库连接成功")
except Exception as e:
    print("数据库连接失败")
    print(e)


# 消息接收回调
def on_message(client, userdata, msg, properties=None):
    print(f"{msg.topic} {msg.payload}")
    # 解析 JSON 数据
    data = json.loads(msg.payload)
    
    # 从解析的数据中提取温度、湿度和照明度
    temperature = data.get("temperature", "")
    humidity = data.get("humidity", "")
    illumination = data.get("illumination", "")
    
    # 将数据插入到数据库
    try:
        # 插入数据的 SQL 语句
        sql = """
        INSERT INTO smart_farm_status(temperature, humidness, illumination)
        VALUES(%s, %s, %s)
        """
        # 使用游标对象执行 SQL 语句并获取结果
        with con.cursor() as cursor:
            cursor.execute(sql, (temperature, humidity, illumination))
            # 插入数据后需要提交
            con.commit()
            print("数据插入成功")
    except Exception as e:
        print("数据插入失败")
        print(e)

# 创建 MQTT 客户端实例
# client = mqtt.Client()
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)


# 设置用户名和密码
client.username_pw_set(username='wds', password='wdsshy0320')

# 注册消息接收回调
client.on_message = on_message

# 建立连接
client.connect('vps.dxh-wds.top', 1883, 60)

# 订阅主题
client.subscribe('smart_farm_status')

# 开始循环监听
client.loop_forever()  # 保持连接