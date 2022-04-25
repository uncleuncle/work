/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-22 10:36
 * Filename      : XLog.h
 * Description   :将打印内容写入指定的文件，参数替代符为<>
 * 例如LOG_DEBUG("aaa<>",1) 则写入[DEBUG——时间] aaa1 
 * *******************************************************/
#include "XLogFun.h"
#include "../XThread/XThread.h"
#include "../common/def.h"
#include <fstream>
#include <queue>

struct LogData
{
    uint32_t logType;
    std::string logMsg;
};

class XLog : public XThread
{

    CREATE_SINGLETON(XLog);
    public:
        virtual bool exec() override;
    public:
        //配置文件目录
        bool init(std::string path);
    public:
        template<typename... Args>
        void errorLog(std::string str, Args... args) {
#ifdef ERROR_L
            insertLog(LOG_ERROR, str, args...);
#endif
        }
        template<typename... Args>
        void warnLog(std::string str, Args... args) {
#ifdef WARN_L
            insertLog(LOG_WARN, str, args...);
#endif
        }
        template<typename... Args>
        void debugLog(std::string str, Args... args) {
#ifdef DEBUG_L
            insertLog(LOG_DEBUG, str, args...);
#endif
        }
        template<typename... Args>
        void insertLog(uint32_t logType, std::string info, Args... args)
        {
            using namespace x_log_fun;
            LogData data;
            data.logType = logType;
            XLogFun::me().getLogInfo(info, args...);
            data.logMsg = info;
            m_queLogs.push(data);
        }
        void writeLog(LogData info);
    private:
        std::ofstream m_ofs;
        std::queue<LogData> m_queLogs;
};
