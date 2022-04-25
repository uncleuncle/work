#include "XLogFun.h"
//------------------------------------------------
//类函数的半特化不能放在类内实现
//放在类内实现在链接的时候会报错
//
//普通函数的模板函数不能放在类外实现
//放在类外实现在链接的时候会报错
//-----------------------------------------------
//定义一个转换字符串的函数模板
//数字类型的转换 int32_t int64_t uint32_t uint64_t
namespace x_log_fun {
    //模板的特化,const char* 类型的转换
    std::string XLogFun::logGetString(const char* s)
    {
        return std::string(s);
    }
    //char类型
    std::string XLogFun::logGetString(char c)
    {
        std::string str(1, c);
        return str;
    }
    //std::string；类型
    std::string XLogFun::logGetString(std::string str)
    {
        return str;
    }
}

//----------------------------------------------
//后续可以根据自己的需求，不断增强自己的log类，比如引入json,proto的打印
