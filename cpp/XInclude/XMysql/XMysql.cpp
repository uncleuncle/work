#include "XMysql.h"
#include <unistd.h>

bool XMysql::init(const char* host, const char* port, const char* user, const char* pwd, const char* db, const uint32_t timeout)
{
    m_host = host;
    m_port = std::atoi(port);
    m_user = user;
    m_pwd = pwd;
    m_db = db;
    m_timeout = timeout;

    if (connect()) {
        return false;
    }

    return true;
}

bool XMysql::exeSql(const char* sql)
{
    if (mysql_real_query(&m_mysql,sql,strlen(sql)) != 0) {
        return false;
    }
   return true;
}

XMysqlRes XMysql::exeSqlStore(const char* sql)
{
    XMysqlRes res;
    check();
    if (!exeSql(sql)) {
        return res;
    }
    //执行成功
    res.suc = true;
    //查询结果
    auto result = mysql_store_result(&m_mysql);
    //行数
    auto rowNum = mysql_num_rows(result);
    //列数
    auto fiedNum = mysql_num_fields(result);
    for (int i = 0; i < rowNum; i++) {
        auto row = mysql_fetch_row(result);
        std::vector<std::string> rows;
        for (int j = 0; j < fiedNum; j++) {
            rows.push_back(row[j]);
        }
        res.res.push_back(rows);
    }

    return res;
}

void XMysql::check()
{
    //ping返回0时表示正常
    if (mysql_ping(&m_mysql) != 0) {
        //关闭mysql
        mysql_close(&m_mysql);
        return;
    }
    
    //重新连接
    connect();
}

bool XMysql::connect()
{
    //初始化
    if (!mysql_init(&m_mysql)) {
        return false;
    }
    //连接数据库
    if (!mysql_real_connect(&m_mysql, m_host.c_str(), m_user.c_str(), m_pwd.c_str(), m_db.c_str(), m_port, NULL, CLIENT_MULTI_STATEMENTS)) {
        return false;
    }
    //设置参数
    mysql_options(&m_mysql, MYSQL_OPT_READ_TIMEOUT, &m_timeout);
    mysql_options(&m_mysql, MYSQL_SET_CHARSET_NAME, "utf8");
}

bool XMysql::exeAsynSql(const char* sql, std::function<void(XMysqlAsynRes*)> fun)
{
    auto info = new XMysqlAsynData;
    info->sql = sql;
    info->fun = fun;
    m_sqlQueue.push(info);
}

bool XMysql::exec()
{
    XMysqlAsynData* info = nullptr;
    while (status()) {
        //没消息的时候让他休息1秒
        if (m_sqlQueue.size() == 0) {
            sleep(1);
            continue;
        }

        if (m_sqlQueue.size() > 0) {
            info = m_sqlQueue.front();
            m_sqlQueue.pop();
            XMysqlRes res;
            check();
            if (!exeSql(info->sql)) {
                continue;
            }
            //执行成功
            res.suc = true;
            //查询结果
            auto result = mysql_store_result(&m_mysql);
            //行数
            auto rowNum = mysql_num_rows(result);
            //列数
            auto fiedNum = mysql_num_fields(result);
            for (int i = 0; i < rowNum; i++) {
                auto row = mysql_fetch_row(result);
                std::vector<std::string> rows;
                for (int j = 0; j < fiedNum; j++) {
                    rows.push_back(row[j]);
                }
                res.res.push_back(rows); 
            }
            XMysqlAsynRes* asynRes = new XMysqlAsynRes();
            asynRes->xMysqlRes = res;
            asynRes->mysqlRes = result;
            mysql_free_result(result);
            if (info->fun) {
                info->fun(asynRes);
            }
        }
    }

    return true;
}
