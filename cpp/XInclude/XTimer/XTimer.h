/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-24 17:08
 * Filename      : XTimer.h
 * Description   : 写一个时钟类，也用单例模式，可以注册两种函数
 * 一种是定时执行的，另一种是间隔执行的 
 * 
 * *******************************************************/

#include "XThread"
#include <functional>
#include <set>

struct TimerData
{
    uint32_t type = 0;
    uint64_t secTime = 0;
    uint32_t beginTime = 0;
    template<typename T>
    std::function<T()> fun;
};

class XTimer: public XThread
{
    CREAT_SINGLETON(XTimer);

    public:
        virtual bool exec() override;
    public:
        template<typename T>
        void regTimerEvent(uint32_t type, uint32_t secTime, std::function<T()> fun)
        {
            TimerData data;
            data.type = type;
            data.secTime = secTime;
            data.fun = fun;
            m_setTimer.push_back(data);
        }
    private:
        std::set<TimerData> m_queueTimer;
};
