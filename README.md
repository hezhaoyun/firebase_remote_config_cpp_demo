# Use Firebase Remote Config on Windows


### Step 1

Download firebase cpp sdk from here:

https://firebase.google.com/download/cpp?authuser=0


### Step 2

Create a Firebase project and add an Android application.

Download the SDK configuration file `google-services.json`.



### Step 3

Clone the current project and put the `google-services.json` file obtained in Step 2 into the project root directory.



### Step 4

If `cmake` is not installed on your computer, install it from https://cmake.org/install/


### Step 5

Config `FIREBASE_CPP_SDK_DIR` to point to `Firebase cpp sdk` folder in `CMakeLists.txt`.


### Step 6

Switch to the project directory under `cmd` and run `cmake .`



### Step 7

Switch to the project directory under `cmd` and run `cmake --build .`



### Step 8

Run `<project directory>/Debug/testapp.exe`



Good luck!

