/**********************************************************
 * Author        : 谢名聪
 * Email         : 869408604@qq.com
 * Last modified : 2022-04-21 11:44
 * Filename      : XConfig.h
 * Description   : 配置类 配置文件的格式为 ：
 * [groupKey1]
 * key1 = value1
 * key2 = value2
 * 
 * [group[key2]
 * key1 = value1
 * key2 = value2
 * *******************************************************/
#ifndef X_CONFG
#define X_CONFG

#include <map>

class XConfig
{
    private:
        std::map<std::string, std::map<std::string, std::string>> m_config;
    public:
        bool load(std::string strPath);
        std::string getConfigValue(std::string strGroupKey, std::string strKey);
};

#endif
