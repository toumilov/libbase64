# libbase64

## Summary
It is a C++ library, which provide Base64 encoding and decoding functionality.

## Requirements
1. GNU gcc compiler
2. GNU Make (or CMake)

## Source code clone
Clone sources into **src** directory, also create **build** directory.
```
git clone <url>
```

## Build
Library uses C++11 features, so the compiler should support that.<br>
Build scripts are written for Make and CMake.
### How to build (Make)
Targets:
* static - build static library libbase64.a
* shared - build shared library libbase64.so
* install - install shared library (_/usr/local/lib/_) and header files (_/usr/local/include/_)
* uninstall - remove library and headers
* test - build and run tests (requires CppUTest package)
* clean - cleanup build folder

### How to build (Cmake)
1. Create build folder
2. Configure and build project in **build** directory.<br>
*_Adjust CMake generator according to your build system(IDE)_
```
mkdir build
cd build
cmake -DSTATIC=ON -DSHARED=ON -DUNITTESTS=OFF -DDEBUG=OFF --configure ../src
cmake --build .
```
Otherwise, generate build files for your IDE.<br>
The example below is using Makefile generator:
```
cmake -G "Unix Makefiles" -D_ECLIPSE_VERSION=4.5 ../src/
```
Or Eclipse MinGW generator:
```
cmake -G "Eclipse CDT4 - MinGW Makefiles" -D_ECLIPSE_VERSION=4.5 ../src/
```
### Tests
To build tests, CppUTest library is required.

### Install
To install shared library and header files, run "_make install_".<br>
Or, for Cmake "_cmake --build . --target install_".

### Uninstall
To remove shared library and header files, run "_make uninstall_".

## Examples
For library usage examples, refer to **test** folder.
