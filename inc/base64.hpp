#pragma once

#include <vector>
#include <string>

class Base64
{
public:
    /**
     * @brief validate Checks whether input string contains valid Base64-encoded data.
     * @param[in] data Base64-encoded string
     * @return True if input is valid Base64 string, or false otherwise.
     */
    static bool validate( const std::string &data );

    /**
     * @brief encode Encodes input binary data into Base64 string.
     * @param[in] data Binary data buffer
     * @param[in] len Input data length
     * @return Base64-encoded string, or empty string in case of error
     */
    static std::string encode( const char *data, unsigned len );

    /**
     * @brief encode_hex Encodes input hex string into Base64 string.
     * @param[in] data Hex string buffer
     * @param[in] len Hex string length (must be even)
     * @return Base64-encoded string, or empty string in case of error
     */
    static std::string encode_hex( const char *data, unsigned len );

    /**
     * @brief decode Decodes input Base64 string to binary data.
     * @param[in] data Base64-encoded string
     * @return Decoded binary data
     */
    static std::vector<char> decode( const std::string &data );

    /**
     * @brief decode_hex Decodes input Base64 string to hex string.
     * @param[in] data Base64-encoded string
     * @return Decoded hex string
     */
    static std::string decode_hex( const std::string &data );

    /**
     * @brief encoded_size Returns size (in bytes) of Base64-encoded buffer.
     * @param[in] size raw(decoded) data size
     * @return data size required for Base64-encoded data
     */
    static unsigned encoded_size( unsigned size );

    /**
     * @brief decoded_size Returns size (in bytes) of Base64 decoded buffer.
     * @param[in] encoded Base64-encoded data
     * @return data size required for decoded Base64 data
     */
    static unsigned decoded_size( const std::string &encoded );
};
