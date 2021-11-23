#ifndef FIREBASE_TESTAPP_MAIN_H_  // NOLINT
#define FIREBASE_TESTAPP_MAIN_H_  // NOLINT

#ifndef FIREBASE_TESTAPP_NAME
#define FIREBASE_TESTAPP_NAME "testapp_main"
#endif  // FIREBASE_TESTAPP_NAME

// Cross platform logging method.
// Implemented by android/android_main.cc or ios/ios_main.mm.
extern "C" void LogMessage(const char* format, ...);

// Platform-independent method to flush pending events for the main thread.
// Returns true when an event requesting program-exit is received.
bool ProcessEvents(int msec);

// WindowContext represents the handle to the parent window.
typedef void* WindowContext;  // A void* for any other environments.

// Returns a variable that describes the window context for the app. On Android
// this will be a jobject pointing to the Activity. On iOS, it's an id pointing
// to the root view of the view controller.

WindowContext GetWindowContext();

#endif  // FIREBASE_TESTAPP_MAIN_H_  // NOLINT
