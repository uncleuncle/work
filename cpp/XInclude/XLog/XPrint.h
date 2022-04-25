/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-23 08:07
 * Filename      : XPrint.h
 * Description   : 写一个自己的打印类，去替代cout，主要可以
 * 增加颜色的控制，用醒目的颜色来标出自己的打印，便于找问题
 * *******************************************************/

#ifndef X_PRINT_H
#define X_PRINT_H

#include "XLogFun.h"
#include<iostream>


class XPrint
{

    CREATE_SINGLETON(XPrint);

    public:
        //错误输出 红色
        template<typename... Args>
        void errorPrint(std::string str, Args... args) {
#ifdef ERROR_P
            using namespace x_log_fun;
            XLogFun::me().getLogInfo(str, args...);
            printf("\033[31m %s\n\033[0m",str.c_str());
#endif
        }
        //警告输出 黄色
        template<typename... Args>
        void warnPrint(std::string str, Args... args) {
#ifdef WARN_P
            using namespace x_log_fun;
            XLogFun::me().getLogInfo(str, args...);
            printf("\033[33m %s\n\033[0m",str.c_str());
#endif
        }
        //调试输出 绿色
        template<typename... Args>
        void debugPrint(std::string str, Args... args) {
#ifdef DEBUG_P
            using namespace x_log_fun;
            XLogFun::me().getLogInfo(str, args...);
            printf("\033[32m %s\n\033[0m",str.c_str());
#endif
        }
};


#endif
