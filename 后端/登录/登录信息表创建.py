'''
Author: wds-dxh wdsnpshy@163.com
Date: 2024-06-06 21:20:43
LastEditors: wds-dxh wdsnpshy@163.com
LastEditTime: 2024-07-17 10:29:53
FilePath: \后端\登录\登录信息表创建.py
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
        database='smart_farm',  #指定数据库
        # cursorclass = pymysql.cursors.DictCursor#返回字典类型
    )
    print("连接成功")
    print(con)

    # 定义建表sql语句
    sql = """
    CREATE TABLE `smart_farm_login` (
    `username` varchar(255) NOT NULL ,
    `password` varchar(255) DEFAULT NULL,
    PRIMARY KEY (`username`)
    ) ENGINE=InnoDB DEFAULT CHARSET=utf8
    """

    #使用游标对象执行sql语句
    con.cursor().execute(sql)

except Exception as e:
    print("连接失败")
    print(e)
finally:
    con.close()

