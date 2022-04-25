#include "XThread.h"

void XThread::run()
{
    m_status = true;
    //开启线程，进入阻塞
    m_future = std::async(std::launch::async, std::bind(&XThread::exec, this));
}

void XThread::stop()
{
    m_status = false;
}

bool XThread::status()
{
    return m_status;
} 
