#include "XLog.h"
#include <time.h>
#include <unistd.h>
//-------------------------------------------
//XLog类函数的实现
//------------------------------------------

//打印当前时间
std::string currentTimeToStr(void){
    char tmp[64];
    time_t t = time(NULL);
    tm *_tm = localtime(&t);
    int year  = _tm->tm_year+1900;
    int month = _tm->tm_mon+1;
    int date  = _tm->tm_mday;
    int hh = _tm->tm_hour;
    int mm = _tm->tm_min;
    int ss = _tm->tm_sec;
    sprintf(tmp,"%04d-%02d-%02d-%02d-%02d-%02d", year,month,date,hh,mm,ss);
    return std::string(tmp);
}

bool XLog::init(std::string path)
{
    std::string file = path + "/LOG_" + currentTimeToStr(); 
    m_ofs.open(file, std::ios::out);
    return true;
}

bool XLog::exec()
{
    while(status()) {
        while (m_queLogs.size()) {
            auto info = m_queLogs.front();
            m_queLogs.pop();
            writeLog(info);
        }
        //处理完之后休息1秒
        sleep(1);
    }
    //关闭文件
    m_ofs.close();
    return true;
}

void XLog::writeLog(LogData info)
{
    std::string strInfo;
    if (info.logType == LOG_ERROR) {
        strInfo = "[ERROR_" + currentTimeToStr() + "]";
    } else if (info.logType == LOG_WARN) {
        strInfo = "[WARN_" + currentTimeToStr() + "]";
    } else if (info.logType == LOG_DEBUG) {
        strInfo = "[DEBUG_" + currentTimeToStr() + "]";
    }
    strInfo += info.logMsg;
    m_ofs << strInfo << std::endl;
}
