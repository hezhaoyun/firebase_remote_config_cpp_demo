#ifndef _WINDOWS_HELPER_H_
#define _WINDOWS_HELPER_H_

#include <direct.h>
#include <windows.h>

#define chdir _chdir

static BOOL WINAPI SignalHandler(DWORD event)
{
    if (!(event == CTRL_C_EVENT || event == CTRL_BREAK_EVENT))
        return FALSE;

    quit = true;

    return TRUE;
}

bool ProcessEvents(int msec)
{
    Sleep(msec);
    return quit;
}

std::string PathForResource()
{
    return std::string();
}

void LogMessage(const char *format, ...)
{
    va_list list;

    va_start(list, format);
    vprintf(format, list);

    va_end(list);
    printf("\n");

    fflush(stdout);
}

WindowContext GetWindowContext() { return nullptr; }

// Change the current working directory to the directory containing the
// specified file.
void ChangeToFileDirectory(const char *file_path)
{
    std::string path(file_path);
    std::replace(path.begin(), path.end(), '\\', '/');

    auto slash = path.rfind('/');

    if (slash != std::string::npos)
    {
        std::string directory = path.substr(0, slash);
        if (!directory.empty())
            chdir(directory.c_str());
    }
}

std::string Unicode2ASCII(const std::string &from)
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

#endif // _WINDOWS_HELPER_H_