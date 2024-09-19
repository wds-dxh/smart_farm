'''
Author: wds-dxh wdsnpshy@163.com
Date: 2024-07-12 08:22:55
LastEditors: wds-dxh wdsnpshy@163.com
LastEditTime: 2024-09-16 12:11:03
FilePath: /Data_persistence/creat_smart_farm.py
Description: 
微信: 15310638214 
邮箱：wdsnpshy@163.com 
Copyright (c) 2024 by ${wds-dxh}, All Rights Reserved. 
'''
from pymysql import Connection

con = None
try:
    # 连接数据库
    con = Connection(
            host='vps.dxh-wds.top',
            user='smartfarm',
            password='smartfarm',
            database='smart_farm',  # 指定数据库
        )
    print("连接成功")
    print(con)


    sql = """
    CREATE TABLE smart_factory_status(
    id INT PRIMARY KEY AUTO_INCREMENT,
    temperature FLOAT,
    humidness FLOAT,
    illumination FLOAT,
    timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8
    """

    #使用游标对象执行sql语句
    con.cursor().execute(sql)


except Exception as e:
    print("连接失败")
    print(e)
finally:
    con.close()
