#ifndef _CONFIG_HELPER_H_
#define _CONFIG_HELPER_H_

#include <string>

#include "firebase/remote_config.h"

typedef ::firebase::remote_config::RemoteConfig RemoteConfig;

namespace channel
{
    const std::string kChannelEdm = "Edm";
    const std::string kChannelXagon = "Xagon";
    const std::string kChannelEfrontier = "Efrontier";
    const std::string kChannelAd = "Ad";
    const std::string kChannelAdX = "Ad_X";
    const std::string kChannelAdContent = "Ad_Content";

    const std::vector<std::string> all{
        kChannelEdm,
        kChannelXagon,
        kChannelEfrontier,
        kChannelAd,
        kChannelAdX,
        kChannelAdContent,
    };
}

namespace version
{
    const std::string kVersionFree = "Free";
    const std::string kVersionTrial = "Trial";

    const std::vector<std::string> all{
        kVersionFree,
        kVersionTrial,
    };
}

namespace language
{
    const std::string kLanguageEnglish = "English";
    const std::string kLanguageChineseTrad = "ChineseTrad";
    const std::string kLanguageDanish = "Danish";
    const std::string kLanguageArabic = "Arabic";
    const std::string kLanguageDutch = "Dutch";
    const std::string kLanguageThai = "Thai";
    const std::string kLanguageMalaysian = "Malaysian";
    const std::string kLanguageFrench = "French";
    const std::string kLanguageGerman = "German";
    const std::string kLanguageItalian = "Italian";
    const std::string kLanguageJapanese = "Japanese";
    const std::string kLanguageKorean = "Korean";
    const std::string kLanguagePolish = "Polish";
    const std::string kLanguagePortuguese = "Portuguese";
    const std::string kLanguageSpanish = "Spanish";

    const std::vector<std::string> all{
        kLanguageEnglish,
        kLanguageChineseTrad,
        kLanguageDanish,
        kLanguageArabic,
        kLanguageDutch,
        kLanguageThai,
        kLanguageMalaysian,
        kLanguageFrench,
        kLanguageGerman,
        kLanguageItalian,
        kLanguageJapanese,
        kLanguageKorean,
        kLanguagePolish,
        kLanguagePortuguese,
        kLanguageSpanish,
    };
}

struct ConfigEnv
{
    std::string channel;
    std::string version;
    std::string language;

    const ConfigEnv(const std::string &c, const std::string &v, const std::string &l)
        : channel(c), version(v), language(l) {}
};

std::string getString(const std::string &key, const std::string defVal, RemoteConfig *rc, const ConfigEnv *env);

long getLong(const std::string &key, long defVal, RemoteConfig *rc, const ConfigEnv *env);

double getDouble(const std::string &key, double defVal, RemoteConfig *rc, const ConfigEnv *env);

bool getBool(const std::string &key, bool defVal, RemoteConfig *rc, const ConfigEnv *env);

#endif // _CONFIG_HELPER_H_