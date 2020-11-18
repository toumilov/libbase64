
#include <algorithm>
#include <cstring>
#include "base64.hpp"


static const std::string mapping_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static void process_chunk( const char *chunk, std::string &result )
{
    result += mapping_[( chunk[0] & 0xfc ) >> 2];
    result += mapping_[( ( chunk[0] & 0x03 ) << 4 ) + ( ( chunk[1] & 0xf0 ) >> 4 )];
    result += mapping_[( ( chunk[1] & 0x0f ) << 2 ) + ( ( chunk[2] & 0xc0 ) >> 6 )];
    result += mapping_[chunk[2] & 0x3f];
}

static bool hex_nibble_to_byte( char ch, char &byte )
{
    if ( ch >= '0' && ch <= '9' )
    {
        byte |= ch - '0';
    }
    else if ( ch >= 'A' && ch <= 'F' )
    {
        byte |= ch - 'A' + 10;
    }
    else if ( ch >= 'a' && ch <= 'f' )
    {
        byte |= ch - 'a' + 10;
    }
    else
    {
        return false;
    }
    return true;
}

static bool hex_byte( char hi, char lo, char &byte )
{
    if ( !hex_nibble_to_byte( hi, byte ) )
    {
        return false;
    }
    byte <<= 4;
    if ( !hex_nibble_to_byte( lo, byte ) )
    {
        return false;
    }
    return true;
}

static bool process_hex_chunk( const char *chunk, std::string &result )
{
    char b1 = 0, b2 = 0, b3 = 0;
    if ( !hex_byte( chunk[0], chunk[1], b1 ) ||
         ( chunk[2] && !hex_byte( chunk[2], chunk[3], b2 ) ) ||
         ( chunk[4] && !hex_byte( chunk[4], chunk[5], b3 ) ) )
    {
        return false;
    }
    result += mapping_[( b1 & 0xfc ) >> 2];
    result += mapping_[( ( b1 & 0x03 ) << 4 ) + ( ( b2 & 0xf0 ) >> 4 )];
    result += mapping_[( ( b2 & 0x0f ) << 2 ) + ( ( b3 & 0xc0 ) >> 6 )];
    result += mapping_[b3 & 0x3f];
    return true;
}

static unsigned index_by_char( char c )
{
    if ( c == '=' )
    {
        return 0;
    }
    auto pos = mapping_.find( c );
    return ( pos == std::string::npos ) ? 0 : pos;
}

static void append_byte_as_hex( std::string &str, char byte )
{
    char tmp = ( byte & 0xf0 ) >> 4;
    str += ( tmp + ( ( tmp > 9 ) ? 'a' - 10 : '0' ) );
    tmp = byte & 0x0f;
    str += ( tmp + ( ( tmp > 9 ) ? 'a' - 10 : '0' ) );
}

bool Base64::validate( const std::string &data )
{
    static const bool ascii_base64[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    if ( ( data.size() % 4 ) != 0 )
    {
        return false;
    }
    for( auto it = data.begin(); it != data.end(); ++it )
    {
        if ( !ascii_base64[(int)*it] )
        {
            return false;
        }
    }
    int num = std::count( data.begin(), data.end(), '=' );
    if ( num > 2 )
    {
        return false;
    }
    for( ; num > 0; num-- )
    {
        if ( data[data.size() - num] != '=' )
        {
            return false;
        }
    }
    return true;
}

std::string Base64::encode( const char *data, unsigned len )
{
    std::string result;
    unsigned result_len = ( ( len * 8u ) / 6u ) + ( ( len * 8u ) % 6u ? 1 : 0 );
    unsigned padding_len = ( ( result_len + 3u ) & ~3u ) - result_len;
    result_len = ( result_len + 3u ) & ~3u;
    result.reserve( result_len );

    const char *p = data;
    for( ; len > 2; len -= 3, p += 3 )
    {
        process_chunk( p, result );
    }
    if ( len )
    {
        char leftover[4] = {0};
        for( unsigned i = 0; len > 0; len--, i++, p++ )
        {
            leftover[i] = *p;
        }
        process_chunk( leftover, result );
        for( unsigned i = 0; i < padding_len; i++ )
        {
            result[result.size() - i - 1] = '=';
        }
    }
    return result;
}

std::string Base64::encode_hex( const char *data, unsigned len )
{
    std::string result;
    if ( len % 2 )
    {
        return result;
    }
    len /= 2;
    unsigned result_len = ( ( len * 8u ) / 6u ) + ( ( len * 8u ) % 6u ? 1 : 0 );
    unsigned padding_len = ( ( result_len + 3u ) & ~3u ) - result_len;
    result_len = ( result_len + 3u ) & ~3u;
    result.reserve( result_len );

    const char *p = data;
    for( ; len > 2; len -= 3, p += 6 )
    {
        if ( !process_hex_chunk( p, result ) )
        {
            return "";
        }
    }
    if ( len )
    {
        char leftover[6] = {0};
        len *= 2;
        for( unsigned i = 0; len > 0; len--, i++, p++ )
        {
            leftover[i] = *p;
        }
        if ( !process_hex_chunk( leftover, result ) )
        {
            return "";
        }
        for( unsigned i = 0; i < padding_len; i++ )
        {
            result[result.size() - i - 1] = '=';
        }
    }
    return result;
}

std::vector<char> Base64::decode( const std::string &data )
{
    std::vector<char> result;
    if ( validate( data ) )
    {
        result.reserve( decoded_size( data ) );
        char buf[4];
        for( unsigned i = 0; i < data.size(); i += 4 )
        {
            buf[0] = index_by_char( data[i] );
            buf[1] = (char)index_by_char( data[i+1] );
            buf[2] = (char)index_by_char( data[i+2] );
            buf[3] = (char)index_by_char( data[i+3] );
            result.push_back( ( buf[0] << 2 ) + ( ( buf[1] & 0x30 ) >> 4 ) );
            result.push_back( ( ( buf[1] & 0x0f ) << 4 ) + ( ( buf[2] & 0x3c ) >> 2 ) );
            result.push_back( ( ( buf[2] & 0x03 ) << 6 ) + ( buf[3] & 0x3f ) );
        }
        auto pos = data.find( "=" );
        switch( pos )
        {
        case std::string::npos:
            break;
        default:
            result.resize( result.size() - ( data.size() - pos ) );
        }
    }
    return result;
}

std::string Base64::decode_hex( const std::string &data )
{
    std::string result;
    if ( validate( data ) )
    {
        result.reserve( decoded_size( data ) );
        char buf[4];
        for( unsigned i = 0; i < data.size(); i += 4 )
        {
            buf[0] = index_by_char( data[i] );
            buf[1] = (char)index_by_char( data[i+1] );
            buf[2] = (char)index_by_char( data[i+2] );
            buf[3] = (char)index_by_char( data[i+3] );
            append_byte_as_hex( result, ( buf[0] << 2 ) + ( ( buf[1] & 0x30 ) >> 4 ) );
            append_byte_as_hex( result, ( ( buf[1] & 0x0f ) << 4 ) + ( ( buf[2] & 0x3c ) >> 2 ) );
            append_byte_as_hex( result, ( ( buf[2] & 0x03 ) << 6 ) + ( buf[3] & 0x3f ) );
        }
        auto pos = data.find( "=" );
        switch( pos )
        {
        case std::string::npos:
            break;
        default:
            result.resize( result.size() - ( data.size() - pos ) * 2  );
        }
    }
    return result;
}

unsigned Base64::encoded_size( unsigned size )
{
    return ( ( size + ( 3 - 1 ) ) / 3 ) * 4;
}

unsigned Base64::decoded_size( const std::string &encoded )
{
    unsigned padding_chars = 0;
    int i = encoded.size() - 1;
    while( true )
    {
        if ( i <= 0 || encoded[i] != '=' )
        {
            break;
        }
        i--;
        padding_chars++;
    }

    return ( ( encoded.size() * 3 ) / 4 ) - padding_chars;
}
