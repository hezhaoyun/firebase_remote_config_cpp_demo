#ifndef _CONFIG_HELPER_H_
#define _CONFIG_HELPER_H_

#include <string>

#include "firebase/remote_config.h"

typedef ::firebase::remote_config::RemoteConfig RemoteConfig;

namespace channel
{
    const char *kChannelEdm = "Edm";
    const char *kChannelXagon = "Xagon";
    const char *kChannelEfrontier = "Efrontier";
    const char *kChannelAd = "Ad";
    const char *kChannelAdX = "Ad_X";
    const char *kChannelAdContent = "Ad_Content";

    const char *all[] = {
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
    const char *kVersionFree = "Free";
    const char *kVersionTrial = "Trial";

    const char *all[] = {
        kVersionFree,
        kVersionTrial,
    };
}

namespace language
{
    const char *kLanguageEnglish = "English";
    const char *kLanguageChineseTrad = "ChineseTrad";
    const char *kLanguageDanish = "Danish";
    const char *kLanguageArabic = "Arabic";
    const char *kLanguageDutch = "Dutch";
    const char *kLanguageThai = "Thai";
    const char *kLanguageMalaysian = "Malaysian";
    const char *kLanguageFrench = "French";
    const char *kLanguageGerman = "German";
    const char *kLanguageItalian = "Italian";
    const char *kLanguageJapanese = "Japanese";
    const char *kLanguageKorean = "Korean";
    const char *kLanguagePolish = "Polish";
    const char *kLanguagePortuguese = "Portuguese";
    const char *kLanguageSpanish = "Spanish";

    const char *all[] = {
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

std::string GetString(const char *key, std::string defaultValue,
                      RemoteConfig *remote_config, char *channel, char *version, char *language);

long GetLong(const char *key, long defaultValue,
             RemoteConfig *remote_config, char *channel, char *version, char *language);

double GetDouble(const char *key, double defaultValue,
                 RemoteConfig *remote_config, char *channel, char *version, char *language);

bool GetBool(const char *key, bool defaultValue,
             RemoteConfig *remote_config, char *channel, char *version, char *language);

#endif // _CONFIG_HELPER_H_