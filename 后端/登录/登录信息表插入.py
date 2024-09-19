'''
Author: wds-dxh wdsnpshy@163.com
Date: 2024-07-20 13:49:52
LastEditors: wds-dxh wdsnpshy@163.com
LastEditTime: 2024-09-16 11:57:43
FilePath: /Smart farm/后端/登录/登录信息表插入.py
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
    # sql = """
    # CREATE TABLE `Smart_factory_login` (
    # `username` varchar(255) NOT NULL ,
    # `password` varchar(255) DEFAULT NULL,
    # PRIMARY KEY (`username`)
    # ) ENGINE=InnoDB DEFAULT CHARSET=utf8
    # """

    sql = """
    insert into smart_farm_login(username,password) 
    values('wds','wds666666')
        """

    # 使用游标对象执行sql语句并获取结果
    with con.cursor() as cursor:
        cursor.execute(sql)
        #插入数据后需要提交
        con.commit()
        print("插入成功")


except Exception as e:
    print("连接失败")
    print(e)
finally:
    con.close()

