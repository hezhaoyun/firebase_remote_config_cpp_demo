# Use Firebase Remote Config on Windows


### Step 0

Download firebase cpp sdk from here:
https://firebase.google.com/download/cpp?authuser=0

### Step 1

Create a Firebase project and add an Android application.

Download the SDK configuration file `google-services.json`.



### Step 2

Clone the current project and put the `google-services.json` file obtained in Step 1 into the project root directory.



### Step 3

If `cmake` is not installed on your computer, install it from https://cmake.org/install/.



### Step 4

Switch to the project directory under `cmd` and run `cmake .`



### Step 5

Switch to the project directory under `cmd` and run `cmake --build .`



### Step 6

Run `<project directory>/Debug/testapp.exe`



good luck!

