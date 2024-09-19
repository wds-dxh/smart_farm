'''
Author: wds-dxh wdsnpshy@163.com
Date: 2024-07-12 08:22:55
LastEditors: wds-dxh wdsnpshy@163.com
LastEditTime: 2024-09-16 12:09:04
FilePath: /Data_persistence/Into_smart_farm.py
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

    # 插入数据的 SQL 语句，包含 temperature、humidness、illumination 和 timestamp 字段
    sql = """
    INSERT INTO smart_farm_status(temperature, humidness, illumination, timestamp)
    VALUES(25, 60, 1000, CURRENT_TIMESTAMP)
    """

    # 使用游标对象执行sql语句并获取结果
    with con.cursor() as cursor:
        cursor.execute(sql)
        # 插入数据后需要提交
        con.commit()
        print("插入成功")


except Exception as e:
    print("连接失败")
    print(e)
finally:
    con.close()
