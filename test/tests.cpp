#include "CppUTest/TestHarness.h"
#include "Serialization/Base64.hpp"

using namespace UtilityLib::Serialization;

TEST_GROUP(Base64Group)
{
    void setup()
    {
    }
    void teardown()
    {
    }

    std::string decode_str( const std::string encoded )
    {
        auto bytes = Base64::decode( encoded );
        return std::string( bytes.begin(), bytes.end() );
    }
};

TEST(Base64Group, Encode)
{
    std::string input( "Test string" );
    auto b64 = Base64::encode( input.c_str(), input.length() );
    STRCMP_EQUAL( "VGVzdCBzdHJpbmc=", b64.c_str() );
    CHECK( Base64::encoded_size( input.size() ) == b64.size() );

    input = "NoPadding";
    b64 = Base64::encode( input.c_str(), input.length() );
    STRCMP_EQUAL( "Tm9QYWRkaW5n", b64.c_str() );
    CHECK( Base64::encoded_size( input.size() ) == b64.size() );

    input = "With Paddings";
    b64 = Base64::encode( input.c_str(), input.length() );
    STRCMP_EQUAL( "V2l0aCBQYWRkaW5ncw==", b64.c_str() );
    CHECK( Base64::encoded_size( input.size() ) == b64.size() );

    input = "";
    b64 = Base64::encode( input.c_str(), input.length() );
    STRCMP_EQUAL( "", b64.c_str() );
    CHECK( Base64::encoded_size( input.size() ) == b64.size() );
}

TEST(Base64Group, EncodeHex)
{
    std::string input( "5465737420737472696e67" ); // Test string
    auto b64 = Base64::encode_hex( input.c_str(), input.length() );
    STRCMP_EQUAL( "VGVzdCBzdHJpbmc=", b64.c_str() );
    CHECK( Base64::encoded_size( input.size() / 2 ) == b64.size() );

    input = "4e6f50616464696e67"; // NoPadding
    b64 = Base64::encode_hex( input.c_str(), input.length() );
    STRCMP_EQUAL( "Tm9QYWRkaW5n", b64.c_str() );
    CHECK( Base64::encoded_size( input.size() / 2 ) == b64.size() );

    input = "576974682050616464696e6773"; // With Paddings
    b64 = Base64::encode_hex( input.c_str(), input.length() );
    STRCMP_EQUAL( "V2l0aCBQYWRkaW5ncw==", b64.c_str() );
    CHECK( Base64::encoded_size( input.size() / 2 ) == b64.size() );

    input = "";
    b64 = Base64::encode_hex( input.c_str(), input.length() );
    STRCMP_EQUAL( "", b64.c_str() );
    CHECK( 0 == b64.size() );
}

TEST(Base64Group, Decode)
{
    std::string input( "VGVzdCBzdHJpbmc=" );
    std::string res( decode_str( input ) );
    std::string expected( "Test string" );
    STRCMP_EQUAL( expected.c_str(), res.c_str() );
    CHECK( Base64::decoded_size( input ) == expected.size() );

    input = "Tm9QYWRkaW5n";
    res = decode_str( input );
    expected = "NoPadding";
    STRCMP_EQUAL( expected.c_str(), res.c_str() );
    CHECK( Base64::decoded_size( input ) == expected.size() );

    input = "V2l0aCBQYWRkaW5ncw==";
    res = decode_str( input );
    expected = "With Paddings";
    STRCMP_EQUAL( expected.c_str(), res.c_str() );
    CHECK( Base64::decoded_size( input ) == expected.size() );

    input = "";
    res = decode_str( input );
    expected = "";
    STRCMP_EQUAL( expected.c_str(), res.c_str() );
    CHECK( Base64::decoded_size( input ) == expected.size() );
}

TEST(Base64Group, DecodeHex)
{
    std::string input( "VGVzdCBzdHJpbmc=" );
    std::string res( Base64::decode_hex( input ) );
    std::string expected( "5465737420737472696e67" ); // Test string
    STRCMP_EQUAL( expected.c_str(), res.c_str() );
    CHECK( Base64::decoded_size( input ) == expected.size() / 2 );

    input = "Tm9QYWRkaW5n";
    res = Base64::decode_hex( input );
    expected = "4e6f50616464696e67"; // NoPadding
    STRCMP_EQUAL( expected.c_str(), res.c_str() );
    CHECK( Base64::decoded_size( input ) == expected.size() / 2 );

    input = "V2l0aCBQYWRkaW5ncw==";
    res = Base64::decode_hex( input );
    expected = "576974682050616464696e6773"; // With Paddings
    STRCMP_EQUAL( expected.c_str(), res.c_str() );
    CHECK( Base64::decoded_size( input ) == expected.size() / 2 );

    input = "";
    res = Base64::decode_hex( input );
    expected = "";
    STRCMP_EQUAL( expected.c_str(), res.c_str() );
    CHECK( Base64::decoded_size( input ) == expected.size() );
}

TEST(Base64Group, Combined)
{
    std::string input( "Hello\nWorld" );
    auto b64 = Base64::encode( input.c_str(), input.length() );
    CHECK( input == Base64::decode( b64 ).data() );
}

TEST(Base64Group, Validate)
{
    CHECK( Base64::validate( "VGVzdCBzdHJpbmc=" ) );
    CHECK( Base64::validate( "ABC=" ) );
    CHECK( Base64::validate( "AB==" ) );
    CHECK( !Base64::validate( "ABC" ) );
    CHECK( !Base64::validate( "AB=C" ) );
}
