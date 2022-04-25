/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-21 11:59
 * Filename      : XProcess.h
 * Description   : 进程的子类，需要实现三个纯虚函数
 * *******************************************************/

#ifndef X_PROCESS
#define X_PROCESS
#include "../Process.h"

class XProcess : public Process
{
    private:
        virtual bool initProcess() override;
        virtual bool startProcess() override;
        virtual bool stopProcess() override;
    private:
        bool m_stop = false;
    public:
        void testAsynSql(XMysqlAsynRes* res);
};

extern XProcess xprocess;
#endif
