/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-21 11:45
 * Filename      : XMysql.h
 * Description   : xsqly类，提供同步异步两种操作
 * *******************************************************/

#ifndef X_MYSQL
#define X_MYSQL

#include "../common/def.h"
#include "../include/mysql/mysql.h"
#include "../XThread/XThread.h"
#include "mysqlToProtobuf.h"
#include <iostream>
#include <vector>
#include <queue>
#include <functional>

struct XMysqlRes
{
    bool suc = false;
    std::vector<std::vector<std::string>> res;
};

struct XMysqlAsynRes
{
    XMysqlAsynRes() = default;
    ~XMysqlAsynRes() {
        //释放掉mysql的数据
        if (mysqlRes) {
            mysql_free_result(mysqlRes);
        }
    }
    XMysqlRes xMysqlRes;
    MYSQL_RES* mysqlRes;
    template<typename PROTO_TYPE>
    bool getProtoRes(std::vector<std::shared_ptr<PROTO_TYPE>>& res) {
        //重置遍历的行数
        mysql_data_seek(mysqlRes, 0);
        mysqlToProtobuf(mysqlRes, res);
    }
};

struct XMysqlAsynData
{
    std::function<void(XMysqlAsynRes*)> fun;
    const char* sql;
};

class XMysql : public XThread
{
    public:
        virtual bool exec() override;
    public:
        //初始化数据库
        bool init(const char* host, const char* port, const char* user, const char* pwd, const char* db, const uint32_t timeout = 2);
    //---------------------------------------------------------
    //同步操作
    //--------------------------------------------------------
    public:
        //有返回的操作
        XMysqlRes exeSqlStore(const char* sql);
        //无返回的操作
        bool exeSql(const char* sql);
        //定义一个模板，针对不同的protobuf结构
        template<typename PROTO_TYPE>
        //这里必须再类内实现，在类外实现的话，进行外部链接的时候会出错
        bool exeSqlProtobuf(const char* sql, std::vector<std::shared_ptr<PROTO_TYPE>>& protoRes)
        {
            if (!exeSql(sql)) {
                return false;
            }
            //转成protobuf形式
            mysqlToProtobuf(mysql_store_result(&m_mysql), protoRes);
            return true;
        }
        //校验连接状态
        void check();
        //连接数据库
        bool connect();
    //---------------------------------------------
    //异步操作
    //---------------------------------------------
    public:
        bool exeAsynSql(const char* sql, std::function<void(XMysqlAsynRes*)> fun);
    private:
        MYSQL m_mysql;
        std::string m_host = "";
        std::string m_user = "";
        std::string m_pwd = "";
        std::string m_db = "";
        uint32_t m_port = 0;
        uint32_t m_timeout = 0;
    //异步处理
    private:
        std::queue<XMysqlAsynData*> m_sqlQueue;//需要处理的队列
};

#endif
