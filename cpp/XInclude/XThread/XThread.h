/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-21 11:43
 * Filename      : XThread.h
 * Description   : 异步线程类
 * *******************************************************/

#ifndef X_THREAD_H
#define X_THREAD_H

#include <atomic>
#include <future>
#include <chrono>

class XThread
{
    public:
        void run();
        void stop();
        bool status();

    private:
        virtual bool exec() = 0;//当状态是false的时候结束线程

    private:
        bool m_status = false;
        std::future<bool> m_future;
};

#endif
