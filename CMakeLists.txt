cmake_minimum_required(VERSION 2.8.12)

# Path to Firebase SDK.
# Try to read the path to the Firebase C++ SDK from an environment variable.
if (NOT "$ENV{FIREBASE_CPP_SDK_DIR}" STREQUAL "")
  set(DEFAULT_FIREBASE_CPP_SDK_DIR "$ENV{FIREBASE_CPP_SDK_DIR}")
else()
  set(DEFAULT_FIREBASE_CPP_SDK_DIR "firebase_cpp_sdk")
endif()

if ("${FIREBASE_CPP_SDK_DIR}" STREQUAL "")
  set(FIREBASE_CPP_SDK_DIR ${DEFAULT_FIREBASE_CPP_SDK_DIR})
endif()

if(NOT EXISTS ${FIREBASE_CPP_SDK_DIR})
  message(FATAL_ERROR "The Firebase C++ SDK directory does not exist: ${FIREBASE_CPP_SDK_DIR}. See the readme.md for more information")
endif()

project(firebase_testapp)

# Sample source files.
set(FIREBASE_SAMPLE_SRCS
  src/config_helper.h
  src/config_helper.cpp
  src/main.cpp
)

# The include directory for the testapp.
include_directories(src)

# Sample uses some features that require C++ 11, such as lambdas.
set (CMAKE_CXX_STANDARD 11)

# Build a desktop application.

# Windows runtime mode, either MD or MT depending on whether you are using
# /MD or /MT. For more information see:
# https://msdn.microsoft.com/en-us/library/2kzt1wy3.aspx
set(MSVC_RUNTIME_MODE MD)

set(target_name "testapp")
add_executable(${target_name}
  ${FIREBASE_SAMPLE_SRCS}
)

set(ADDITIONAL_LIBS advapi32 ws2_32 crypt32 rpcrt4 ole32)

# If a config file is present, copy it into the binary location so that it's
# possible to create the default Firebase app.
set(FOUND_JSON_FILE FALSE)

foreach(config "google-services-desktop.json" "google-services.json")
  if (EXISTS ${config})
    add_custom_command(
      TARGET ${target_name} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
        ${config} $<TARGET_FILE_DIR:${target_name}>)
    set(FOUND_JSON_FILE TRUE)
    break()
  endif()
endforeach()

if(NOT FOUND_JSON_FILE)
  message(WARNING "Failed to find either google-services-desktop.json or google-services.json.")
endif()

# Add the Firebase libraries to the target using the function from the SDK.
add_subdirectory(${FIREBASE_CPP_SDK_DIR} bin/ EXCLUDE_FROM_ALL)

# Note that firebase_app needs to be last in the list.
set(firebase_libs firebase_remote_config firebase_app)
target_link_libraries(${target_name} "${firebase_libs}" ${ADDITIONAL_LIBS})
