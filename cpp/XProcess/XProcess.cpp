#include "XProcess.h"
#include "../../proto/src/User.pb.h"
#include  <unistd.h>
#include <time.h>
XProcess xprocess;

bool XProcess::initProcess()
{
    //加载数据库
    std::string host = getConfigValue("mysql", "host");
    std::string port = getConfigValue("mysql", "port");
    std::string user = getConfigValue("mysql", "user");
    std::string pwd = getConfigValue("mysql", "pwd");
    std::string db = getConfigValue("mysql", "db");
    std::string sqlType = getConfigValue("mysql", "type");
    uint32_t type = std::atoi(sqlType.c_str());
    if (!addMysqlServer(type, host.c_str(), port.c_str(), user.c_str(), pwd.c_str(), db.c_str())) {
        SYS_ERROR_PRINT("addMysqlServer error");
        return false;
    }

    //加载异步数据库
    if (!addAsynMysqlServer(type, host.c_str(), port.c_str(), user.c_str(), pwd.c_str(), db.c_str())) {
        SYS_ERROR_PRINT("addAsynMysqlServer error");
        return false;
    }

    //加载服务名字和类型
    std::string serverName = getConfigValue("server", "name");
    std::string serverType = getConfigValue("server", "type");
    std::string serverId = getConfigValue("server", "id");

    m_name = serverName;
    m_type = std::atoi(serverType.c_str());
    m_id = std::atoi(serverId.c_str());
    
    //加载配置
    std::string path = getConfigValue("server", "log_path");
    if (!initLog(path)) {
        SYS_ERROR_PRINT("init log error");
        return false;
    }
    return true;
}

bool XProcess::startProcess()
{
    while (m_stop == false) {
        /*
        //获取系统时间戳
        time_t timeReal;
        time(&timeReal);
        timeReal = timeReal + 8 * 3600;
        tm* t = gmtime(&timeReal); 
        printf("%d-%02d-%02d %02d:%02d:%02d\n", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec); 
        */
        /*
        std::string sql = "select * from user_data;";
        SYS_DEBUG_PRINT("exeAsynSql():<>",sql); 
        exeAsynSql(1, sql.c_str(), std::bind(&XProcess::testAsynSql, this, std::placeholders::_1));
        */
        SYS_ERROR_LOG("error日志 <> <> ",100,1);
        SYS_WARN_LOG("Warn日志 <> <>", 200,'c');
        SYS_DEBUG_LOG("Debug日志 <>", "ssssssssss");
        
        SYS_ERROR_PRINT("error日志 <> <> ",100,1);
        SYS_WARN_PRINT("Warn日志 <> <>", 200,'c');
        SYS_DEBUG_PRINT("Debug日志 <>", "ssssssssss");


        sleep(1);

    };
    return true;
}

bool XProcess::stopProcess()
{
    SYS_DEBUG_PRINT("stop  <>", m_name); 
    m_stop = true;
    return true;
}

void XProcess::testAsynSql(XMysqlAsynRes* res)
{
    if (!res) {
        SYS_WARN_PRINT("xmysqlAsyRes == nullptr");
    }
    SYS_DEBUG_PRINT("XMysqlRes ------------");
    auto xRes = res->xMysqlRes;
    for (auto res : xRes.res) {
        for (auto row : res) {
            SYS_DEBUG_PRINT("<>", row);
        }
        std::cout << std::endl;
    }
    SYS_DEBUG_PRINT("protoRes------------------- ");
    std::vector<std::shared_ptr<User>> users;
    res->getProtoRes(users);
    for (auto u : users) {
        SYS_DEBUG_PRINT("<>, <> ", u->id(), u->nickname());
    }

}
