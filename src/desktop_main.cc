#include <signal.h>
#include <stdarg.h>
#include <stdio.h>

#include <direct.h>
#define chdir _chdir

#include <windows.h>

#include <algorithm>
#include <string>

#include "main.h" // NOLINT

// The TO_STRING macro is useful for command line defined strings as the quotes
// get stripped.
#define TO_STRING_EXPAND(X) #X
#define TO_STRING(X) TO_STRING_EXPAND(X)

// Path to the Firebase config file to load.
#ifdef FIREBASE_CONFIG
#define FIREBASE_CONFIG_STRING TO_STRING(FIREBASE_CONFIG)
#else
#define FIREBASE_CONFIG_STRING ""
#endif // FIREBASE_CONFIG

extern "C" int common_main(int argc, const char *argv[]);

static bool quit = false;

static BOOL WINAPI SignalHandler(DWORD event)
{
  if (!(event == CTRL_C_EVENT || event == CTRL_BREAK_EVENT)) return FALSE;

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

// Returns the number of microseconds since the epoch.

int64_t WinGetCurrentTimeInMicroseconds()
{
  FILETIME file_time;
  GetSystemTimeAsFileTime(&file_time);

  ULARGE_INTEGER now;
  now.LowPart = file_time.dwLowDateTime;
  now.HighPart = file_time.dwHighDateTime;

  // Windows file time is expressed in 100s of nanoseconds.
  // To convert to microseconds, multiply x10.

  return now.QuadPart * 10LL;
}

int main(int argc, const char *argv[])
{
  ChangeToFileDirectory(FIREBASE_CONFIG_STRING[0] != '\0' ? FIREBASE_CONFIG_STRING : argv[0]); // NOLINT
  SetConsoleCtrlHandler((PHANDLER_ROUTINE) SignalHandler, TRUE);

  return common_main(argc, argv);
}