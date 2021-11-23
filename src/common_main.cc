#include <assert.h>

#include "firebase/app.h"
#include "firebase/remote_config.h"
#include "firebase/util.h"

// Thin OS abstraction layer.
#include "main.h" // NOLINT

// Convert remote_config::ValueSource to a string.
const char *ValueSourceToString(firebase::remote_config::ValueSource source)
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
    LogMessage("Try to initialize Remote Config");
    ::firebase::remote_config::RemoteConfig::GetInstance(app);

    return ::firebase::InitResult::kInitResultSuccess;
}

// Execute all methods of the C++ Remote Config API.

extern "C" int common_main(int argc, const char *argv[])
{
    ::firebase::App *app;

    // Initialization

    LogMessage("Initialize the Firebase Remote Config library");
    app = ::firebase::App::Create();

    LogMessage("Created the Firebase app %x", static_cast<int>(reinterpret_cast<intptr_t>(app)));

    ::firebase::ModuleInitializer initializer;
    initializer.Initialize(app, nullptr, init_fn);

    while (initializer.InitializeLastResult().status() != firebase::kFutureStatusComplete)
    {
        if (ProcessEvents(100))
            return 1; // exit if requested
    }

    if (initializer.InitializeLastResult().error() != 0)
    {
        LogMessage("Failed to initialize Firebase Remote Config: %s", initializer.InitializeLastResult().error_message());
        ProcessEvents(2000);
        return 1;
    }

    LogMessage("Initialized the Firebase Remote Config API");

    ::firebase::remote_config::RemoteConfig *remote_config = ::firebase::remote_config::RemoteConfig::GetInstance(app);

    // Initialization Complete
    // Set Defaults, and test them

    static const unsigned char kBinaryDefaults[] = {6, 7, 3};
    static const ::firebase::remote_config::ConfigKeyValueVariant defaults[] = {
        {"menu_show_extendshrink", "True"},
        {"language_support", ""},
        {"TestData", firebase::Variant::FromStaticBlob(kBinaryDefaults, sizeof(kBinaryDefaults))}};

    size_t default_count = sizeof(defaults) / sizeof(defaults[0]);
    remote_config->SetDefaults(defaults, default_count);

    // The return values may not be the set defaults, if a fetch was previously
    // completed for the app that set them.

    ::firebase::remote_config::ValueInfo value_info;

    {
        bool result = remote_config->GetBoolean("menu_show_extendshrink", &value_info);
        LogMessage("Get menu_show_extendshrink %d %s", result ? 1 : 0, ValueSourceToString(value_info.source));
    }

    {
        std::string result = remote_config->GetString("language_support", &value_info);
        LogMessage("Get language_support \"%s\" %s", result.c_str(), ValueSourceToString(value_info.source));
    }

    {
        std::vector<unsigned char> result = remote_config->GetData("TestData");
        for (size_t i = 0; i < result.size(); ++i)
        {
            const unsigned char value = result[i];
            LogMessage("TestData[%d] = 0x%02x", i, value);
        }
    }

    {
        std::string result = remote_config->GetString("TestNotSet", &value_info);
        LogMessage("Get TestNotSet \"%s\" %s", result.c_str(), ValueSourceToString(value_info.source));
    }

    // Test the existence of the keys by name.
    // Print out the keys with default values.

    std::vector<std::string> keys = remote_config->GetKeys();
    LogMessage("GetKeys:");
    for (auto s = keys.begin(); s != keys.end(); ++s)
    {
        LogMessage("  %s", s->c_str());
    }

    keys = remote_config->GetKeysByPrefix("TestD");
    LogMessage("GetKeysByPrefix(\"TestD\"):");
    for (auto s = keys.begin(); s != keys.end(); ++s)
    {
        LogMessage("  %s", s->c_str());
    }

    // Test Fetch...

    LogMessage("Fetch...");
    auto future_result = remote_config->Fetch(0);
    while (future_result.status() == firebase::kFutureStatusPending)
    {
        if (ProcessEvents(1000))
            break;
    }

    if (future_result.status() == firebase::kFutureStatusComplete)
    {
        LogMessage("Fetch Complete");
        bool activate_result = remote_config->Activate().result();
        LogMessage("ActivateFetched %s", activate_result ? "succeeded" : "failed");

        const ::firebase::remote_config::ConfigInfo &info = remote_config->GetInfo();
        LogMessage(
            "Info last_fetch_time_ms=%d (year=%.2f) fetch_status=%d "
            "failure_reason=%d throttled_end_time=%d",
            static_cast<int>(info.fetch_time),
            1970.0f + static_cast<float>(info.fetch_time) / (1000.0f * 60.0f * 60.0f * 24.0f * 365.0f),
            info.last_fetch_status, info.last_fetch_failure_reason,
            info.throttled_end_time);

        // Print out the new values, which may be updated from the Fetch.

        {
            bool result = remote_config->GetBoolean("menu_show_extendshrink", &value_info);
            LogMessage("Updated menu_show_extendshrink %d %s", result ? 1 : 0, ValueSourceToString(value_info.source));
        }

        {
            std::string result = remote_config->GetString("language_support", &value_info);
            LogMessage("Updated language_support \"%s\" %s", result.c_str(), ValueSourceToString(value_info.source));
        }

        {
            std::vector<unsigned char> result = remote_config->GetData("TestData");
            for (size_t i = 0; i < result.size(); ++i)
            {
                const unsigned char value = result[i];
                LogMessage("TestData[%d] = 0x%02x", i, value);
            }
        }

        {
            std::string result = remote_config->GetString("TestNotSet", &value_info);
            LogMessage("Get TestNotSet \"%s\" %s", result.c_str(), ValueSourceToString(value_info.source));
        }

        // Print out the keys that are now tied to data

        std::vector<std::string> keys = remote_config->GetKeys();
        LogMessage("GetKeys:");
        for (auto s = keys.begin(); s != keys.end(); ++s)
        {
            LogMessage("  %s", s->c_str());
        }

        keys = remote_config->GetKeysByPrefix("TestD");
        LogMessage("GetKeysByPrefix(\"TestD\"):");
        for (auto s = keys.begin(); s != keys.end(); ++s)
        {
            LogMessage("  %s", s->c_str());
        }
    }
    else
    {
        LogMessage("Fetch Incomplete");
    }

    // Release a handle to the future so we can shutdown the Remote Config API
    // when exiting the app.

    future_result.Release();

    // Wait until the user wants to quit the app.
    while (!ProcessEvents(1000))
    {
    }

    // remote_config->Terminate();
    delete app;

    return 0;
}
