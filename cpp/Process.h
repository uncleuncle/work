/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-21 11:42
 * Filename      : Process.h
 * Description   : 进程的基类
 * *******************************************************/

#ifndef PROCESS_H
#define PROCESS_H

#include "XInclude/common/def.h"
#include "XInclude/XMysql/XMysql.h"
#include "XInclude/XLog/XLog.h"
#include "XInclude/XLog/XPrint.h"
#include "XInclude/XConfig/XConfig.h"


class Process
{
    public:
        Process();
        virtual ~Process();

    //进程子类需要实现的逻辑
    private:
        virtual bool initProcess() = 0;
        virtual bool startProcess() = 0;
        virtual bool stopProcess() = 0;

    //main.cpp中调用的逻辑
    public:
        bool init(const std::string config);
        bool start();
        bool stop();
        bool startAsynTools();
        bool stopAsynTools();
    //*********************************************************
    //工具类:包括config socket  redis mysql timer log等
    //*********************************************************

    //---------------------------------------------------------
    //config
    //--------------------------------------------------------
    private:
        std::shared_ptr<XConfig> m_config;
    public:
        //获取配置
        std::string getConfigValue(const std::string groupKey, const std::string key);

    //----------------------------------------------------------
    //Log类 
    //---------------------------------------------------------
    public:
        bool initLog(std::string path);

    //-----------------------------------------------------------
    //同步数据库，只操作一些简单的逻辑
    //-----------------------------------------------------------
    private:
        std::map<uint32_t, std::shared_ptr<XMysql>> m_XMysqls;
    public:
        //增加数据库服务
        bool addMysqlServer(uint32_t id, const char* host, \
                const char* port, const char* user, \
                const char* pwd, const char* db, \
                uint32_t timeout = 2);
        //无返回值
        bool exeSql(uint32_t id, const char* sql); 
        //返回XMysql
        XMysqlRes exeSqlStore(uint32_t id, const char* sql);
        //返回protobuf
        template<typename PROTO_TYPE>
        bool exeSqlProtobuf(uint32_t id, const char* sql, std::vector<std::shared_ptr<PROTO_TYPE>>& protoRes)
        {
            auto it = m_XMysqls.find(id);
            if (it == m_XMysqls.end()) {
                return false;
            }

            return m_XMysqls[id]->exeSqlProtobuf(sql, protoRes);
        } 
    //------------------------------------------------------------
    //异步mysql 异步类型  有一些不需要马上得到结果的操作可以在这完成
    //-----------------------------------------------------------
    private:
        std::map<uint32_t, std::shared_ptr<XMysql>> m_asynXMysqls;
    public:
        //增加一个异步mysql服务
        bool addAsynMysqlServer(uint32_t id, const char* host, \
                const char* port, const char* user, \
                const char* pwd, const char* db, \
                uint32_t timeout = 20);
        //加入一个异步操作
        bool exeAsynSql(uint32_t id, const char* sql,  std::function<void (XMysqlAsynRes*)> func = nullptr);

    public:
        std::string m_name = "";
        uint32_t m_type = 0;
        uint32_t m_id = 0;
        std::vector<std::shared_ptr<XThread>> m_asynTools;
};
extern Process* g_process;

#endif
