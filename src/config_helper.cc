#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "config_helper.h"

std::string toLower(const std::string &originStr)
{
    std::string str = originStr;

    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                   { return std::tolower(c); });

    return str;
}

bool inArray(const std::string &str, const char *array[])
{
    for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++)
    {
        if (toLower(str) == array[i])
        {
            return true;
        }
    }

    return false;
}

std::vector<std::string> split(const std::string &str, const char splitChar)
{
    std::istringstream stream(str);

    std::string token;
    std::vector<std::string> tokens;

    while (getline(stream, token, splitChar))
    {
        tokens.push_back(token);
    }

    return tokens;
}

std::string SelectKey(const std::vector<std::string> &keys, const char *key,
                      const char *channel, const char *version, const char *language)
{
    for (auto itKey = keys.begin(); itKey != keys.end(); itKey++)
    {
        auto tokens = split(*itKey, '.');

        // key 的主体部分被包含在找到的 key 中，但不相同

        if (toLower(*itKey) != key)
            return "";

        // 仅有 key 主体部分

        if (tokens.size() == 1)
            return *itKey;

        auto itToken = tokens.begin();
        itToken++;

        for (; itToken != tokens.end(); itToken++)
        {
            // 通道不匹配

            if (inArray(*itToken, channel::all))
            {
                if (toLower(*itToken) != channel)
                    continue;
            }

            // 版本不匹配

            if (inArray(*itToken, version::all))
            {
                if (toLower(*itToken) != version)
                    continue;
            }

            // 语言不匹配

            if (inArray(*itToken, language::all))
            {
                if (toLower(*itToken) != language)
                    continue;
            }
        }

        return *itKey;
    }

    return "";
}

std::string GetString(const char *key, std::string defaultValue,
                      RemoteConfig *remote_config, char *channel, char *version, char *language)
{
    auto matchKey = SelectKey(remote_config->GetKeysByPrefix(key), key, channel, version, language);

    if (matchKey.size() > 0)
    {
        return remote_config->GetString(matchKey.c_str());
    }

    return defaultValue;
}

long GetLong(const char *key, long defaultValue,
             RemoteConfig *remote_config, char *channel, char *version, char *language)
{
    auto matchKey = SelectKey(remote_config->GetKeysByPrefix(key), key, channel, version, language);

    if (matchKey.size() > 0)
    {
        return remote_config->GetLong(matchKey.c_str());
    }

    return defaultValue;
}

double GetDouble(const char *key, double defaultValue,
                 RemoteConfig *remote_config, char *channel, char *version, char *language)
{
    auto matchKey = SelectKey(remote_config->GetKeysByPrefix(key), key, channel, version, language);

    if (matchKey.size() > 0)
    {
        return remote_config->GetDouble(matchKey.c_str());
    }

    return defaultValue;
}

bool GetBool(const char *key, bool defaultValue,
             RemoteConfig *remote_config, char *channel, char *version, char *language)
{
    auto matchKey = SelectKey(remote_config->GetKeysByPrefix(key), key, channel, version, language);

    if (matchKey.size() > 0)
    {
        return remote_config->GetBoolean(matchKey.c_str());
    }

    return defaultValue;
}
