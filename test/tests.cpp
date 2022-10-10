#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include "base64.hpp"

using namespace base64;

int main( int argc, char** argv)
{
	MemoryLeakWarningPlugin::turnOffNewDeleteOverloads();
	return CommandLineTestRunner::RunAllTests( argc, argv );
}

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
		auto bytes = decode( encoded );
		return std::string( bytes.begin(), bytes.end() );
	}

	std::string decode_hex_str( const std::string encoded )
	{
		auto bytes = decode_hex( encoded );
		return std::string( bytes.begin(), bytes.end() );
	}
};

TEST(Base64Group, Encode)
{
	std::string input( "\x12\x34\x56\x55" );
	auto b64 = encode( input.c_str(), input.length() );
	STRCMP_EQUAL( "EjRWVQ==", b64.c_str() );
	CHECK( encoded_size( input.size() ) == b64.size() );

	input = "Test string";
	b64 = encode( input.c_str(), input.length() );
	STRCMP_EQUAL( "VGVzdCBzdHJpbmc=", b64.c_str() );
	CHECK( encoded_size( input.size() ) == b64.size() );

	input = "NoPadding";
	b64 = encode( input.c_str(), input.length() );
	STRCMP_EQUAL( "Tm9QYWRkaW5n", b64.c_str() );
	CHECK( encoded_size( input.size() ) == b64.size() );

	input = "With Paddings";
	b64 = encode( input.c_str(), input.length() );
	STRCMP_EQUAL( "V2l0aCBQYWRkaW5ncw==", b64.c_str() );
	CHECK( encoded_size( input.size() ) == b64.size() );

	input = "";
	b64 = encode( input.c_str(), input.length() );
	STRCMP_EQUAL( "", b64.c_str() );
	CHECK( encoded_size( input.size() ) == b64.size() );
}

TEST(Base64Group, EncodeHex)
{
	std::string input( "12345655" );
	auto b64 = encode_hex( input.c_str(), input.length() );
	STRCMP_EQUAL( "EjRWVQ==", b64.c_str() );
	CHECK( encoded_size( input.size() / 2 ) == b64.size() );

	input = "5465737420737472696e67"; // Test string
	b64 = encode_hex( input.c_str(), input.length() );
	STRCMP_EQUAL( "VGVzdCBzdHJpbmc=", b64.c_str() );
	CHECK( encoded_size( input.size() / 2 ) == b64.size() );

	input = "4e6f50616464696e67"; // NoPadding
	b64 = encode_hex( input.c_str(), input.length() );
	STRCMP_EQUAL( "Tm9QYWRkaW5n", b64.c_str() );
	CHECK( encoded_size( input.size() / 2 ) == b64.size() );

	input = "576974682050616464696e6773"; // With Paddings
	b64 = encode_hex( input.c_str(), input.length() );
	STRCMP_EQUAL( "V2l0aCBQYWRkaW5ncw==", b64.c_str() );
	CHECK( encoded_size( input.size() / 2 ) == b64.size() );

	input = "";
	b64 = encode_hex( input.c_str(), input.length() );
	STRCMP_EQUAL( "", b64.c_str() );
	CHECK( 0 == b64.size() );
}

TEST(Base64Group, Decode)
{
	std::string input( "VGVzdCBzdHJpbmc=" );
	auto res = decode( input );
	std::string expected( "Test string" );
	STRNCMP_EQUAL( expected.c_str(), res.data(), res.size() );
	CHECK( decoded_size( input ) == expected.size() );

	input = "Tm9QYWRkaW5n";
	res = decode( input );
	expected = "NoPadding";
	STRNCMP_EQUAL( expected.c_str(), res.data(), res.size() );
	CHECK( decoded_size( input ) == expected.size() );

	input = "V2l0aCBQYWRkaW5ncw==";
	res = decode( input );
	expected = "With Paddings";
	STRNCMP_EQUAL( expected.c_str(), res.data(), res.size() );
	CHECK( decoded_size( input ) == expected.size() );

	input = "";
	res = decode( input );
	CHECK( res.empty() );
}

TEST(Base64Group, DecodeHex)
{
	std::string input( "VGVzdCBzdHJpbmc=" );
	auto res = decode_hex( input );
	std::string expected( "5465737420737472696e67" ); // Test string
	STRNCMP_EQUAL( expected.c_str(), res.data(), res.size() );
	CHECK( decoded_size( input ) == expected.size() / 2 );

	input = "Tm9QYWRkaW5n";
	res = decode_hex( input );
	expected = "4e6f50616464696e67"; // NoPadding
	STRNCMP_EQUAL( expected.c_str(), res.data(), res.size() );
	CHECK( decoded_size( input ) == expected.size() / 2 );

	input = "V2l0aCBQYWRkaW5ncw==";
	res = decode_hex( input );
	expected = "576974682050616464696e6773"; // With Paddings
	STRNCMP_EQUAL( expected.c_str(), res.data(), res.size() );
	CHECK( decoded_size( input ) == expected.size() / 2 );

	input = "=";
	res = decode_hex( input );
	CHECK( res.empty() );

	input = "";
	res = decode_hex( input );
	CHECK( res.empty() );
}

TEST(Base64Group, Combined)
{
	std::string input( "Hello\nWorld" );
	auto b64 = encode( input.c_str(), input.length() );
	CHECK( input == decode( b64 ).data() );
}

TEST(Base64Group, Validate)
{
	CHECK( validate( "VGVzdCBzdHJpbmc=" ) );
	CHECK( validate( "ABC=" ) );
	CHECK( validate( "AB==" ) );
	CHECK( !validate( "ABC" ) );
	CHECK( !validate( "AB=C" ) );
	CHECK( !validate( "=" ) );
	CHECK( !validate( "====" ) );
}

TEST(Base64Group, Encoder)
{
	Encoder e;
	CHECK( e );
	std::string chunk( "Test" );
	STRCMP_EQUAL( "VGVz", e.encode( chunk.c_str(), chunk.size() ).c_str() );
	CHECK( e );
	chunk = "20"; // " "
	STRCMP_EQUAL( "", e.encode_hex( chunk.c_str(), chunk.size() ).c_str() );
	CHECK( e );
	chunk = "737472696E"; // "strin"
	STRCMP_EQUAL( "dCBzdHJp", e.encode_hex( chunk.c_str(), chunk.size() ).c_str() );
	CHECK( e );
	chunk = "g";
	STRCMP_EQUAL( "", e.encode( chunk.c_str(), chunk.size() ).c_str() );
	CHECK( e );
	STRCMP_EQUAL( "bmc=", e.finalize().c_str() );
	CHECK( e );
}

TEST(Base64Group, Decoder)
{
	Decoder d;
	std::vector<char> result;
	CHECK( d );
	std::string chunk( "VGVzdCBzdHJ" );
	CHECK( d.decode( chunk.c_str(), chunk.size(), result ) );
	STRNCMP_EQUAL( "Test s", (const char*)result.data(), result.size() );
	CHECK( d );
	CHECK_FALSE( d.done() );
	chunk = "pbmc=";
	CHECK( d.decode( chunk.c_str(), chunk.size(), result ) );
	STRNCMP_EQUAL( "Test string", (const char*)result.data(), result.size() );
	CHECK( d );
	CHECK( d.done() );

	d.reset();
	chunk = "abc@";
	CHECK_FALSE( d.decode( chunk.c_str(), chunk.size(), result ) );
	CHECK_FALSE( d );

	d.reset();
	chunk = "==ab";
	CHECK_FALSE( d.decode( chunk.c_str(), chunk.size(), result ) );
	CHECK_FALSE( d );
}

TEST(Base64Group, DecoderHex)
{
	Decoder d;
	std::vector<char> result;
	CHECK( d );
	std::string chunk( "VGVzdCBzdHJ" );
	CHECK( d.decode_hex( chunk.c_str(), chunk.size(), result ) );
	STRNCMP_EQUAL( "546573742073", (const char*)result.data(), result.size() );
	CHECK( d );
	CHECK_FALSE( d.done() );
	chunk = "pbmc=";
	CHECK( d.decode_hex( chunk.c_str(), chunk.size(), result ) );
	STRNCMP_EQUAL( "5465737420737472696e67", (const char*)result.data(), result.size() );
	CHECK( d );
	CHECK( d.done() );
}
