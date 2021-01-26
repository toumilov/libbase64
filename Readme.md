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
### Headers
Library API is defined in _base64_ namespace.
```
#include <base64.hpp>
```
### Base64 validation
Base64 validation functions are available for std::string and C strings:
```
std::string b64_str( "VGVzdCBzdHJpbmc=" );
if ( base64::validate( b64 ) )
{
    // validation successful
}
const char *b64 = "VGVzdCBzdHJpbmc=";
if ( base64::validate( b64, strlen( b64 ) ) )
{
    // validation successful
}
```
### Data encoding
Calculating base64 encoding output size for _n_ bytes:
```
unsigned b64_characters = base64::encoded_size( n );
```
Raw binary data encoding:
```
std::string b64 = base64::encode( data, size );
```
Hex data encoding (size should be even):
```
std::string b64 = base64::encode_hex( data, size );
if ( b64.empty() )
{
    // failed to encode
}
```
Encoding data stream (in chunks):
```
base64::Encoder e;
auto encoded = e.encode( chunk1.c_str(), chunk1.size() ) ); // Encode data portion
if ( e )
{
    // ecnoding successful
}
encoded += e.encode( chunk2.c_str(), chunk2.size() ) );     // Encode some more data
encoded += e.encode_hex( chunk3.c_str(), chunk3.size() ) ); // Encode data in hex
encoded += e.finalize(); // get trailing characters
e.reset(); // cleanup encoder to prepare for another encoding round
```
### Data decoding
Calculating buffer size (in bytes) required for decoded base64 data:
```
unsigned b64_characters = base64::decoded_size( "VGVzdCBzdHJpbmc=" );
```
Base64 std::string decoding to binary
```
std::vector<char> bytes = base64::decode( "VGVzdCBzdHJpbmc=" );
if ( bytes.empty() )
{
    // decoding failed
}
```
Base64 C string decoding to binary
```
const char *b64 = "VGVzdCBzdHJpbmc=";
std::vector<char> bytes = base64::decode( b64, strlen( b64 ) );
if ( bytes.empty() )
{
    // decoding failed
}
```
Base64 std::string decoding to hex string
```
std::vector<char> bytes = base64::decode_hex( "VGVzdCBzdHJpbmc=" );
if ( bytes.empty() )
{
    // decoding failed
}
```
Base64 C string decoding to hex string
```
const char *b64 = "VGVzdCBzdHJpbmc=";
std::vector<char> bytes = base64::decode_hex( b64, strlen( b64 ) );
if ( bytes.empty() )
{
    // decoding failed
}
```
Decoding data stream (in chunks):
```
base64::Decoder d;
std::vector<char> out;
do
{
    // decode binary chunk
    d.decode( chunk, chunk.size(), out );
    // or, decode chunk to hex string
    d.decode_hex( chunk, chunk.size(), out );
} while( d && !d.done() );
if ( !d )
{
    // decoding failed
}
d.reset(); // reset decoder state
```

