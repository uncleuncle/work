#include "XConfig.h"
#include "../XLog/XLog.h"
#include "../XLog/XPrint.h"
#include <iostream>
#include <fstream>
#include <vector>

bool XConfig::load(std::string strPath)
{
    std::ifstream file;
    file.open(strPath);

    SYS_DEBUG_PRINT("-------------------load config begin------------------");
    std::string buf;
    std::vector<std::string> vConfig;
    while(getline(file, buf)) {
        std::string value;
        //将配置的元素拆分成字符串
        for (auto c : buf) {
            if (c == ' ') {
                continue;
            } else if (c == '#') {
                break;
            } else if (c == ']'){
                value += c;
                break;
            } else if (c == '=') {
                vConfig.push_back(value);
                value = '=';
                continue;
            }
            value += c;
        }
        if (value.size() > 0) {
            vConfig.push_back(value);
        }
    }

    int nIndex = 0;
    std::string strGroupKey = "";
    std::string strKey = "";
    while (nIndex < vConfig.size()) {
        auto& strConfig = vConfig[nIndex++];
        //配置组名
        if (strConfig[0] == '[') {
            if (strConfig[strConfig.size() - 1] != ']') {
                SYS_ERROR_PRINT("config error:", strConfig);
                return false;
            } else {
                strGroupKey = strConfig.substr(1, strConfig.size() - 2);
            }
            continue;
        }
        //配置的值
        if (strConfig[0] == '=') {
            if (strGroupKey.size() == 0 || strKey.size() == 0) {
                SYS_ERROR_PRINT("config error:", strConfig);
                return false;
            }
            m_config[strGroupKey][strKey] = strConfig.substr(1, strConfig.size() - 1);
            strKey = "";
            continue;
        }
        //配置的名字
        {
            if (strGroupKey.size() == 0) {
                SYS_ERROR_PRINT("config groupKey empty:", strConfig);
                return false;
            }
            if (strKey.size() != 0) {
                SYS_ERROR_PRINT("config strKey not empty:", strConfig);
                return false;

            }
            strKey = strConfig;
        }
    }

    file.close();

    //打印加载的结果
    for (auto itGroup = m_config.begin(); itGroup != m_config.end(); itGroup++) {
        for (auto it = itGroup->second.begin(); it != itGroup->second.end(); it++) {
            SYS_DEBUG_PRINT("m_config[<>][<>]=<>", itGroup->first, it->first, it->second);
        }
    }
    SYS_DEBUG_PRINT("-------------------load config end------------------");
    return true;
}

std::string XConfig::getConfigValue(std::string strGroupKey, std::string strKey)
{
    auto itGroup = m_config.find(strGroupKey);
    if (itGroup == m_config.end()) {
        return "";
    }

    auto it = itGroup->second.find(strKey);
    if (it == itGroup->second.end()) {
        return "";
    }
    return it->second;

}
