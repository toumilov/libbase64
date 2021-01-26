#pragma once

#include <vector>
#include <string>

namespace base64
{

/**
 * @brief validate Checks whether input string contains valid Base64-encoded data.
 * @param[in] data Base64-encoded string
 * @return True if input is valid Base64 string, otherwise returns false.
 */
bool validate( const std::string &data );

/**
 * @brief validate Checks whether input buffer contains valid Base64-encoded data.
 * @param[in] data Base64-encoded string
 * @param[out] size Base64-encoded string length
 * @return True if input is valid Base64 string, otherwise returns false.
 */
bool validate( const char *data, unsigned size );

/**
 * @brief encode Encodes input binary data into Base64 string.
 * @param[in] data Binary data buffer
 * @param[in] size Input data length
 * @return Base64-encoded string, or empty string in case of error
 */
std::string encode( const char *data, unsigned size );

/**
 * @brief encode_hex Encodes input hex string into Base64 string.
 * @param[in] data Hex string buffer
 * @param[in] size Hex string length (must be even)
 * @return Base64-encoded string, or empty string in case of error
 */
std::string encode_hex( const char *data, unsigned size );

/**
 * @brief decode Decodes input Base64 string to binary data.
 * @param[in] data Base64-encoded string
 * @return Decoded binary data, or empty vector if error occurred
 */
std::vector<char> decode( const std::string &data );

/**
 * @brief decode Decodes input Base64 string to binary data.
 * @param[in] data Base64-encoded string
 * @param[in] size Base64-encoded string length
 * @return Decoded binary data, or empty vector if error occurred
 */
std::vector<char> decode( const char *data, unsigned size );

/**
 * @brief decode_hex Decodes input Base64 string to hex string.
 * @param[in] data Base64-encoded string
 * @return Decoded hex string, or empty vector if error occurred
 */
std::vector<char> decode_hex( const std::string &data );

/**
 * @brief decode_hex Decodes input Base64 string to hex string.
 * @param[in] data Base64-encoded string
 * @param[in] size Base64-encoded string length
 * @return Decoded hex string, or empty vector if error occurred
 */
std::vector<char> decode_hex( const char *data, unsigned size );

/**
 * @brief encoded_size Returns size (in bytes) of Base64-encoded buffer.
 * @param[in] size raw(decoded) data size
 * @return data size required for Base64-encoded data
 */
unsigned encoded_size( unsigned size );

/**
 * @brief decoded_size Returns size (in bytes) of Base64 decoded buffer.
 * @param[in] encoded Base64-encoded data
 * @return data size required for decoded Base64 data, or 0 if bad data specified
 */
unsigned decoded_size( const std::string &encoded );

/**
 * @brief decoded_size Returns size (in bytes) of Base64 decoded buffer.
 * @param[in] encoded Base64-encoded data
 * @param[in] size Base64-encoded string length
 * @return data size required for decoded Base64 data, or 0 if bad data specified
 */
unsigned decoded_size( const char *encoded, unsigned size );


/**
 * Encoder class for chunked encoding
 */
class Encoder
{
public:
	Encoder();

	/**
	 * @brief operator bool Returns true, if decoding is successful.
	 */
	operator bool() const;

	/**
	 * @brief reset Clears object state.
	 * @return object reference
	 */
	Encoder& reset();

	/**
	 * @brief encode Encodes data chunk to Base64.
	 * @param[in] data Data to encode
	 * @param[in] size Data length
	 * @return encoded chunk data
	 */
	std::string encode( const char *data, unsigned size );

	/**
	 * @brief encode_hex Encodes hex string chunk to Base64.
	 * @param[in] data Data to encode
	 * @param[in] size Data length
	 * @return encoded chunk data
	 */
	std::string encode_hex( const char *data, unsigned size );

	/**
	 * @brief finalize Finalize encoding (encode leftover).
	 * @return encoded leftover data
	 */
	std::string finalize();

private:
	bool status_;
	unsigned encoded_bytes_;
	unsigned n_;
	char chunk_[3];
};


/**
 * Decoder class for chunked decoding
 */
class Decoder
{
public:
	Decoder();

	/**
	 * @brief operator bool Returns true, if decoding is successful.
	 */
	operator bool() const;

	/**
	 * @brief done Returns true, if decoding is complete.
	 * @return true if internal buffer is empty
	 */
	bool done() const;

	/**
	 * @brief reset Clears object state.
	 * @return object reference
	 */
	Decoder& reset();

	/**
	 * @brief decode Decodes Base64 chunk to bytes.
	 * @param[in] data Base64-encoded data
	 * @param[in] size Base64-encoded string length
	 * @param[out] output data continer
	 * @return true, if decoding is successful
	 */
	bool decode( const char *data, unsigned size, std::vector<char> &out );

	/**
	 * @brief decode_hex Decodes Base64 chunk to hex string.
	 * @param[in] data Base64-encoded data
	 * @param[in] size Base64-encoded string length
	 * @param[out] output data continer
	 * @return true, if decoding is successful
	 */
	bool decode_hex( const char *data, unsigned size, std::vector<char> &out );

private:
	bool status_;
	bool done_;
	unsigned n_;
	char chunk_[4];

	bool decode_( const char *data, unsigned size, std::vector<char> &out, unsigned n, void (*append)( std::vector<char>&, char ) );
};

}; // base64
