#include "Process.h"

Process* g_process = nullptr;

Process::Process()
{
    if (!g_process) {
        g_process = this; 
    }
    if (!m_config) {
        m_config = std::make_shared<XConfig>();
    }
}

Process::~Process()
{
}

bool Process::init(const std::string config)
{
    try {
        // 加载配置
        if (!m_config->load(config)) {
            SYS_ERROR_PRINT("load config error");
            return false;
        }
        // 初始化进程
        if (!initProcess()) {
            SYS_ERROR_PRINT("initProcess error!");
            return false;
        }
        //开启异步工具
        startAsynTools();

    } catch (std::exception& ex) {
        SYS_WARN_PRINT("Process, init catch exception");
    }

    SYS_DEBUG_PRINT("Process, init success!");
    return true;
}

bool Process::start()
{
    try {
    // 开启服务逻辑
    if (!startProcess()) {
        SYS_ERROR_PRINT("startProcess error!");
        return false;
    }
    } catch (std::exception& ex) {
        SYS_WARN_PRINT("PROCESS, start catch exception");
    }

    SYS_DEBUG_PRINT("PROCESS, start success!");

    return true;
}

bool Process::stop()
{
    try {
        //关闭工具
        stopAsynTools();
        //关闭服务逻辑
        stopProcess();
        //关闭服务打印
        SYS_DEBUG_PRINT("PROCESS, <> <>" ,m_name, "stopped");
    } catch (std::exception& ex) {
        SYS_WARN_PRINT("stop ex, ",ex.what());
    }

    return true;
}

std::string Process::getConfigValue(const std::string groupKey, const std::string key)
{
    return m_config->getConfigValue(groupKey, key);
}

bool Process::addMysqlServer(uint32_t id, const char* host, \
        const char* port, const char* user, \
        const char* pwd, const char* db, \
        uint32_t timeout)
{
    auto it = m_XMysqls.find(id);
    if (it != m_XMysqls.end()) {
        return true;
    }
    //创建mysql服务链接
    auto mysql = std::make_shared<XMysql> ();
    m_XMysqls[id] = mysql;
    return mysql->init(host, port, user, pwd, db, timeout);
}

bool Process::exeSql(uint32_t id, const char* sql)
{
    auto it = m_XMysqls.find(id);
    if (it == m_XMysqls.end()) {
        return false;
    }
    //同步执行sql语句
    return m_XMysqls[id]->exeSql(sql);
}


XMysqlRes Process::exeSqlStore(uint32_t id, const char* sql)
{
    auto it = m_XMysqls.find(id);
    if (it == m_XMysqls.end()) {
        return XMysqlRes();
    }
    //执行同步sql语句
    return m_XMysqls[id]->exeSqlStore(sql);
}

bool Process::addAsynMysqlServer(uint32_t id, const char* host, \
        const char* port, const char* user, \
        const char* pwd, const char* db, \
        uint32_t timeout)
{
    auto it = m_asynXMysqls.find(id);
    if (it != m_asynXMysqls.end()) {
        return true;
    }
    auto mysql = std::make_shared<XMysql> ();
    if (!mysql->init(host, port, user, pwd, db, timeout)) {
        SYS_ERROR_PRINT("init asynSql error");
        return false;
    }
    //创建异步mysql服务链接
    m_asynXMysqls[id] = mysql;
    m_asynTools.push_back(mysql);
    return true;
}

bool Process::exeAsynSql(uint32_t id, const char* sql, std::function<void(XMysqlAsynRes*)> func)
{
    auto it = m_asynXMysqls.find(id);
    if (it == m_asynXMysqls.end()) {
        SYS_ERROR_PRINT("not find m_asynXMsql id = ", id);
        return false;
    }
    //执行异步sql语句
    return m_asynXMysqls[id]->exeAsynSql(sql, func);
}

bool Process::startAsynTools()
{
    //开启日志
    XLog::me().run();
    //开启异步工具
    for (auto tool : m_asynTools) {
        tool->run();
    }
    return true;
}


bool Process::stopAsynTools()
{
    //关闭异步工具
    for (auto tool : m_asynTools) {
        tool->stop();
    }
    //关闭日志
    XLog::me().stop();

    return true;
}

bool Process::initLog(std::string path)
{
    //初始化日志系统
    if (!XLog::me().init(path)) {
        SYS_ERROR_PRINT("log init error");
        return false;
    }

    return true;
}
