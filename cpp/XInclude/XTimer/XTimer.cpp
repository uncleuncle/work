#include "../../common"
#include "XTimer.h"
#include <ctime.h>
#include <unist.h>

bool XTimer::exec()
{
    while (status()) {
        while (m_queueTimer.size()) {
            auto info = m_queueTimer.front();
            m_queueTimer.pop();
            uint64_t unixTime = std::time(NULL);
            if (info.secTime + info.beginTime >= unixTime) {
                info.fun();
                if (info.type == TIME_TYPE_REPEAT) {
                    info.beginTime = unixTime;
                    m_queueTimer.push_back(info);
                }
            }
        }
        sleep(1);
    }
    return true;
}
