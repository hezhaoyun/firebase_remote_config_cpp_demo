#include <vector>
#include <sstream>
#include <iostream>
#include <algorithm>

#include "config_helper.h"
#define FieldSpliter "__"

std::string toLower(const std::string &origin)
{
    std::string str = origin;

    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                   { return std::tolower(c); });

    return str;
}

bool inArray(const std::string &str, const std::vector<std::string> &array)
{
    for (int i = 0; i < array.size(); i++)
    {
        if (toLower(str) == toLower(array[i]))
        {
            return true;
        }
    }

    return false;
}

std::vector<std::string> split(const std::string &originStr, const std::string &spliter)
{
    std::string str = originStr;
    std::vector<std::string> tokens;

    char *token = strtok((char *)str.c_str(), spliter.c_str());

    while (token != NULL)
    {
        tokens.push_back(token);
        token = strtok(NULL, spliter.c_str());
    }

    return tokens;
}

std::string selectKey(const std::vector<std::string> originKeys, const std::string &key, const ConfigEnv *env)
{
    // Sort keys by condition field amount

    std::vector<std::string> keys = originKeys;

    std::sort(keys.begin(), keys.end(), [](const std::string &a, const std::string &b)
              { return split(a, FieldSpliter).size() > split(b, FieldSpliter).size(); });

    for (auto itKey = keys.begin(); itKey != keys.end(); itKey++)
    {
        auto tokens = split(*itKey, FieldSpliter);

        // key's first token must as same as search

        if (toLower(tokens[0]) != toLower(key))
            return "";

        // has key body alone

        if (tokens.size() == 1)
            return key;

        auto itToken = tokens.begin();

        itToken++;
        auto matchBreak = false;

        for (; itToken != tokens.end(); itToken++)
        {
            // channel is not matched.

            if (inArray(*itToken, channel::all))
            {
                if (toLower(*itToken) != toLower(env->channel))
                {
                    matchBreak = true;
                    break;
                }
            }

            // version is not matched.

            if (inArray(*itToken, version::all))
            {
                if (toLower(*itToken) != toLower(env->version))
                {
                    matchBreak = true;
                    break;
                }
            }

            // lanuguage is not matched.

            if (inArray(*itToken, language::all))
            {
                if (toLower(*itToken) != toLower(env->language))
                {
                    matchBreak = true;
                    break;
                }
            }
        }

        if (matchBreak)
        {
            continue;
        }

        return *itKey;
    }

    return "";
}

std::string getString(const std::string &key, std::string defVal, RemoteConfig *rc, const ConfigEnv *env)
{
    auto matchKey = selectKey(rc->GetKeysByPrefix(key.c_str()), key, env);

    if (matchKey.size() > 0)
    {
        return rc->GetString(matchKey.c_str());
    }

    return defVal;
}

long getLong(const std::string &key, long defVal, RemoteConfig *rc, const ConfigEnv *env)
{
    auto matchKey = selectKey(rc->GetKeysByPrefix(key.c_str()), key, env);

    if (matchKey.size() > 0)
    {
        return rc->GetLong(matchKey.c_str());
    }

    return defVal;
}

double getDouble(const std::string &key, double defVal, RemoteConfig *rc, const ConfigEnv *env)
{
    auto matchKey = selectKey(rc->GetKeysByPrefix(key.c_str()), key, env);

    if (matchKey.size() > 0)
    {
        return rc->GetDouble(matchKey.c_str());
    }

    return defVal;
}

bool getBool(const std::string &key, bool defVal, RemoteConfig *rc, const ConfigEnv *env)
{
    auto matchKey = selectKey(rc->GetKeysByPrefix(key.c_str()), key, env);

    if (matchKey.size() > 0)
    {
        return rc->GetBoolean(matchKey.c_str());
    }

    return defVal;
}
