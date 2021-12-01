#include <signal.h>
#include <stdarg.h>
#include <stdio.h>

#include <algorithm>
#include <string>

#include "firebase/app.h"
#include "firebase/remote_config.h"
#include "firebase/util.h"

#include "config_helper.h"

#include <direct.h>
#include <windows.h>

static bool quit = false;

std::string unicode2Ascii(const std::string &from)
{
    std::string to;

    int len = MultiByteToWideChar(CP_UTF8, 0, from.c_str(), -1, nullptr, 0);
    std::vector<wchar_t> buffer(len, 0);
    MultiByteToWideChar(CP_UTF8, 0, from.c_str(), -1, buffer.data(), len);

    len = WideCharToMultiByte(CP_ACP, 0, buffer.data(), -1, nullptr, 0, nullptr, nullptr);
    std::vector<char> temp(len, 0);
    WideCharToMultiByte(CP_ACP, 0, buffer.data(), -1, temp.data(), len, nullptr, nullptr);

    to = temp.data();
    return to;
}

// Convert remote_config::ValueSource to a string.

const char *valueSourceToString(firebase::remote_config::ValueSource source)
{
    static const char *kSourceToString[] = {
        "Static",  // kValueSourceStaticValue
        "Remote",  // kValueSourceRemoteValue
        "Default", // kValueSourceDefaultValue
    };

    return kSourceToString[source];
}

::firebase::InitResult init_fn(::firebase::App *app, void *)
{
    printf("Try to initialize Remote Config...\n");
    ::firebase::remote_config::RemoteConfig::GetInstance(app);
    return ::firebase::InitResult::kInitResultSuccess;
}

// Execute all methods of the C++ Remote Config API.

int main(int argc, const char *argv[])
{
    ::firebase::App *app;

    // Initialization

    printf("Initialize the Firebase Remote Config library ...\n");
    app = ::firebase::App::Create();

    ::firebase::ModuleInitializer initializer;
    initializer.Initialize(app, nullptr, init_fn);

    while (initializer.InitializeLastResult().status() != firebase::kFutureStatusComplete)
    {
        Sleep(100);
    }

    if (initializer.InitializeLastResult().error() != 0)
    {
        printf("Failed to initialize Firebase Remote Config: %s", initializer.InitializeLastResult().error_message());
        Sleep(2000);
        return 1;
    }

    printf("Initialized the Firebase Remote Config API ...\n");

    ::firebase::remote_config::RemoteConfig *remote_config = ::firebase::remote_config::RemoteConfig::GetInstance(app);

    // Initialization Complete, set Defaults, and test them

    static const unsigned char kBinaryDefaults[] = {6, 7, 3};
    static const ::firebase::remote_config::ConfigKeyValueVariant defaults[] = {
        {"rate", 0.85},
        {"json_iap", "{}"}, // process default value in logic
        {"url_homepage", "https://update.easeus.com/data-wiper.php"},
        {"url_upgrade_check", "https://update.easeus.com/update/disktoolkit/update.ini"},
        {"url_privacy_policy", "https://www.easeus.com/easeus-customer-service-privacy-statement.html"},
    };

    size_t default_count = sizeof(defaults) / sizeof(defaults[0]);
    remote_config->SetDefaults(defaults, default_count);

    // Test Fetch...

    ::firebase::remote_config::ValueInfo value_info;
    printf("\n\nFetch...\n");

    auto future_result = remote_config->Fetch(0);
    while (future_result.status() == firebase::kFutureStatusPending)
    {
        Sleep(1000);
    }

    if (future_result.status() == firebase::kFutureStatusComplete)
    {
        printf("Fetch Complete\n");

        bool activate_result = remote_config->Activate().result();
        printf("ActivateFetched %s\n\n\n", activate_result ? "succeeded" : "failed");

        // const ::firebase::remote_config::ConfigInfo &info = remote_config->GetInfo();
        // printf(
        //     "Info last_fetch_time_ms=%d fetch_status=%d failure_reason=%d throttled_end_time=%d\n",
        //     static_cast<int>(info.fetch_time),
        //     info.last_fetch_status,
        //     info.last_fetch_failure_reason,
        //     info.throttled_end_time
        // );

        // Print out the keys that are now tied to data

        // std::vector<std::string> keys = remote_config->GetKeys();
        // printf("GetKeys:\n");
        // for (auto s = keys.begin(); s != keys.end(); ++s)
        // {
        //     printf("  %s\n", s->c_str());
        // }

        // keys = remote_config->GetKeysByPrefix("TestD");
        // printf("GetKeysByPrefix(\"TestD\"):");
        // for (auto s = keys.begin(); s != keys.end(); ++s)
        // {
        //     printf("  %s\n", s->c_str());
        // }

        // Print out the new values, which may be updated from the Fetch.

        auto v = unicode2Ascii(remote_config->GetString("json_iap", &value_info));
        printf("json_iap: %s %s\n", v.c_str(), valueSourceToString(value_info.source));
        
        v = remote_config->GetString("url_homepage", &value_info);
        printf("url_homepage: \"%s\" %s\n", v.c_str(), valueSourceToString(value_info.source));
        
        v = remote_config->GetString("url_upgrade_check", &value_info);
        printf("url_upgrade_check: \"%s\" %s\n", v.c_str(), valueSourceToString(value_info.source));
        
        v = remote_config->GetString("url_privacy_policy", &value_info);
        printf("url_privacy_policy: \"%s\" %s\n\n", v.c_str(), valueSourceToString(value_info.source));

        // Complex param test: manage channel, version, languge by self

        v = getString("rate", "-", remote_config, &ConfigEnv("", "Free", "English"));
        printf("rate (Free + English): %s\n", v.c_str());

        v = getString("rate", "-", remote_config, &ConfigEnv("", "Trial", "German"));
        printf("rate (Frial + German): %s\n", v.c_str());

        v = getString("rate", "-", remote_config, &ConfigEnv("xagon", "Free", "French"));
        printf("rate (Xagon + Free + French): %s\n\n\n", v.c_str());
    }
    else
    {
        printf("Fetch Incomplete\n");
    }

    // Release a handle to the future so we can shutdown the Remote Config API
    // when exiting the app.

    future_result.Release();

    delete remote_config;
    delete app;

    return 0;
}