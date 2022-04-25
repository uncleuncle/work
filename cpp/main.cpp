/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-21 11:42
 * Filename      : main.cpp
 * Description   : 程序的入口
 * *******************************************************/
#include "Process.h"

#include <iostream>
#include <signal.h>
#include <sys/stat.h>

void sig_handler(int sig)
{
    //接收到信号关闭进程
    g_process->stop();
}

void set_signal_handlers(void)
{
    signal(SIGHUP, SIG_IGN);
    signal(SIGPIPE, SIG_IGN);

    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = sig_handler;
    sigaction(SIGTERM, &act, NULL);
    sigaction(SIGINT, &act, NULL);
}

int main(int argc, char** argv)
{
    //没有传配置文件
    if (argc < 2) {
        SYS_ERROR_PRINT("need config");
        return -1;
    }

    //没有初始化g_process
    if (g_process == nullptr) {
        SYS_ERROR_PRINT("g_process == nullptr");
        return -1;
    }

    //加载配置
    if (!g_process->init(argv[1])) {
        SYS_ERROR_PRINT("g_process init error");
        return -1;
    }

    //当接收到关闭信号，则关闭程序
    set_signal_handlers(); 

    //主进程
    if (!g_process->start()) {
        SYS_ERROR_PRINT("process error");
        return -1;
    }

    SYS_DEBUG_PRINT("main.cpp stop");
    return 0;
}
