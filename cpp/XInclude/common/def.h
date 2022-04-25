/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-21 11:44
 * Filename      : def.h
 * Description   : 一些通用的宏定义，类的定义放在这里 
 * *******************************************************/
#ifndef BASE_DEF_H
#define BASE_DEF_H
//------------------------------------------
//日志类型
#define LOG_ERROR 1
#define LOG_WARN 2
#define LOG_DEBUG 3

#define SYS_ERROR_LOG XLog::me().errorLog
#define SYS_WARN_LOG XLog::me().warnLog
#define SYS_DEBUG_LOG XLog::me().debugLog

//--------------------------------------------
//输出
#define SYS_ERROR_PRINT XPrint::me().errorPrint
#define SYS_WARN_PRINT XPrint::me().warnPrint
#define SYS_DEBUG_PRINT XPrint::me().debugPrint

//-------------------------------------------
//时钟类型
#define TIME_TYPE_NULL 0
#define TIME_TYPE_REPEAT 1
#define TIME_TYPE_WAIT 2

//单例模式的宏
//将构造函数 拷贝构造函数设置为私有
#define CREATE_SINGLETON(CLASS_TYPE) \
private:\
    CLASS_TYPE(){}\
    CLASS_TYPE(const CLASS_TYPE & type){}\
public:\
    static CLASS_TYPE & me()\
    {\
        static CLASS_TYPE me;\
        return me;\
    }

#endif
