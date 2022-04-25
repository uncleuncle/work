/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-22 15:25
 * Filename      : XLogFun.h
 * Description   : 根据参数获得string
 * *******************************************************/
#ifndef X_LOG_FUN
#define X_LOG_FUN
#include "../common/def.h"
#include <iostream>
namespace x_log_fun {
    class XLogFun
    {
        CREATE_SINGLETON(XLogFun);

        public:
            std::string logGetString(const char* s);
            std::string logGetString(char c);
            std::string logGetString(std::string str);
            template<typename T>
            std::string logGetString(T t)
            {
                return std::to_string(t);
            }


            //定义一个插入参数的参数
            //使用到了变长函数模板
            void insertValue(int index, std::string& str)
            {
                //递归结束
                return;
            }

            template <typename T, typename... Args>
            void insertValue(int index, std::string& str, T t, Args...args)
            {
                while (index + 1 < str.size()) {
                    if (str[index] == '<'  && str[index + 1] == '>' ) {
                        str = str.substr(0, index) + logGetString(t) + str.substr(index + 2, str.size() - 1);
                        insertValue(index++, str, args...);
                        return;
                    }
                    index++;
                }
                insertValue(0, str);
            }

            //返回函数
            template<typename... Args>
            void getLogInfo(std::string& str, Args... args)
            {
                return insertValue(0, str, args...);
            }
    };
}

#endif

