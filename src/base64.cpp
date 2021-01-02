
#include <algorithm>
#include <cstring>
#include "base64.hpp"

namespace base64
{

static const char mapping_[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const unsigned char valid_base64_characters_[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,63, 0, 0, 0,64,53,54,55,56,57,58,59,60,61,62, 0, 0, 0, 0, 0, 0,
	0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26, 0, 0, 0, 0, 0,
	0,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const char hex_characters_[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 0, 0, 0, 0, 0,
	0, 11, 12, 13, 14, 15, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 11, 12, 13, 14, 15, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static bool process_chunk( const char *&chunk, std::string &result, unsigned inc )
{
	result += mapping_[( chunk[0] & 0xfc ) >> 2];
	result += mapping_[( ( chunk[0] & 0x03 ) << 4 ) + ( ( chunk[1] & 0xf0 ) >> 4 )];
	result += mapping_[( ( chunk[1] & 0x0f ) << 2 ) + ( ( chunk[2] & 0xc0 ) >> 6 )];
	result += mapping_[chunk[2] & 0x3f];
	chunk += inc;
	return true;
}

static inline bool hex_nibble_to_byte( char ch, char &byte )
{
	if ( hex_characters_[(int)ch] )
	{
		byte |= hex_characters_[(int)ch] - 1;
		return true;
	}
	return false;
}

static bool hex_byte( char hi, char lo, char &byte )
{
	byte = 0;
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

static bool process_hex_chunk( const char *&chunk, std::string &result, unsigned inc )
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
	chunk += inc;
	return true;
}

static inline unsigned index_by_char( char c )
{
	if ( c == '=' )
	{
		return 0;
	}
	auto n = valid_base64_characters_[c];
	return ( n > 0 ) ? n - 1 : 0;
}

bool validate( const std::string &data )
{
	return validate( data.c_str(), data.size() );
}

bool validate( const char *data, unsigned size )
{
	if ( size % 4 || size == 0 )
	{
		return false;
	}
	unsigned i;
	for( i = 0; i < size; i++ )
	{
		char ch = data[i];
		if ( ch == '=' )
		{
			break;
		}
		if ( valid_base64_characters_[(unsigned)ch] == 0 )
		{
			return false;
		}
	}
	if ( i == 0 )
	{
		return false;
	}
	for( unsigned eq = 0; i < size; i++ )
	{
		if ( data[i] != '=' )
		{
			break;
		}
		eq++;
		if ( eq > 2 )
		{
			return false;
		}
	}
	return data[i] == '\0';
}

static std::string encode_( const char *data, unsigned size, unsigned n, bool (*process_chunk_)( const char*&, std::string&, unsigned ) )
{
	std::string result;
	unsigned result_len = ( ( size * 8u ) / 6u ) + ( ( size * 8u ) % 6u ? 1 : 0 );
	unsigned padding_len = ( ( result_len + 3u ) & ~3u ) - result_len;
	result_len = ( result_len + 3u ) & ~3u;
	result.reserve( result_len );

	const char *p = data;
	for( ; size > 2; size -= 3 )
	{
		if ( !process_chunk_( p, result, n ) )
		{
			return std::string();
		}
	}
	if ( size )
	{
		char leftover[n] = {0};
		size *= 2;
		for( unsigned i = 0; size > 0; size--, i++, p++ )
		{
			leftover[i] = *p;
		}
		p = leftover;
		if ( !process_chunk_( p, result, 0 ) )
		{
			return std::string();
		}
		for( unsigned i = 0; i < padding_len; i++ )
		{
			result[result.size() - i - 1] = '=';
		}
	}
	return result;
}

std::string encode( const char *data, unsigned size )
{
	return encode_( data, size, 3, &process_chunk );
}

std::string encode_hex( const char *data, unsigned size )
{
	if ( size % 2 )
	{
		return std::string();
	}
	size /= 2;
	return encode_( data, size, 6, &process_hex_chunk );
}

static void append( std::vector<char> &v, char ch )
{
	v.push_back( ch );
}

static void append_hex( std::vector<char> &v, char ch )
{
	char tmp = ( ch & 0xf0 ) >> 4;
	v.push_back( tmp + ( ( tmp > 9 ) ? 'a' - 10 : '0' ) );
	tmp = ch & 0x0f;
	v.push_back( tmp + ( ( tmp > 9 ) ? 'a' - 10 : '0' ) );
}

std::vector<char> decode_( const char *data, unsigned size, unsigned n, void (*append)( std::vector<char>&, char ) )
{
	std::vector<char> result;
	if ( validate( data, size ) )
	{
		result.reserve( decoded_size( data, size ) );
		char buf[4];
		for( unsigned i = 0; i < size; i += 4 )
		{
			char buf[] = {
				(char)index_by_char( data[i] ),
				(char)index_by_char( data[i+1] ),
				(char)index_by_char( data[i+2] ),
				(char)index_by_char( data[i+3] )
			};
			append( result, ( buf[0] << 2 ) + ( ( buf[1] & 0x30 ) >> 4 ) );
			append( result, ( ( buf[1] & 0x0f ) << 4 ) + ( ( buf[2] & 0x3c ) >> 2 ) );
			append( result, ( ( buf[2] & 0x03 ) << 6 ) + ( buf[3] & 0x3f ) );
		}
		unsigned eq;
		for( eq = 0; data[size - eq - 1] == '='; eq++ );
		switch( eq )
		{
		case 0:
			break;
		default:
			result.resize( result.size() - eq * n );
		}
	}
	return result;
}

std::vector<char> decode( const std::string &data )
{
	return decode( data.c_str(), data.size() );
}

std::vector<char> decode( const char *data, unsigned size )
{
	return decode_( data, size, 1, &append );
}

std::vector<char> decode_hex( const std::string &data )
{
	return decode_hex( data.c_str(), data.size() );
}

std::vector<char> decode_hex( const char *data, unsigned size )
{
	return decode_( data, size, 2, &append_hex );
}

unsigned encoded_size( unsigned size )
{
	return ( ( size + ( 3 - 1 ) ) / 3 ) * 4;
}

unsigned decoded_size( const std::string &encoded )
{
	return decoded_size( encoded.c_str(), encoded.size() );
}

unsigned decoded_size( const char *encoded, unsigned size )
{
	unsigned padding_chars;
	if ( size == 0 || encoded[0] == '=' )
	{
		return 0;
	}
	for( padding_chars = 0; encoded[size - padding_chars - 1] == '='; padding_chars++ );
	return ( ( size * 3 ) / 4 ) - padding_chars;
}


Encoder::Encoder() :
	status_( true ),
	encoded_bytes_( 0 ),
	n_( 0 )
{
}

Encoder::operator bool() const
{
	return status_;
}

void Encoder::reset()
{
	status_ = true;
	encoded_bytes_ = 0;
	n_ = 0;
}

std::string Encoder::encode( const char *data, unsigned size )
{
	if ( !status_ )
	{
		return std::string();
	}
	encoded_bytes_ += size;
	unsigned pos = 0;
	std::string result;
	while( true )
	{
		for( ; n_ < 3 && pos < size; n_++, pos++ )
		{
			chunk_[n_] = data[pos];
		}
		if ( n_ < 3 )
		{
			break;
		}
		const char *p = chunk_;
		if ( !process_chunk( p, result, 0 ) )
		{
			status_ = false;
			return std::string();
		}
		n_ = 0;
	}
	return result;
}

std::string Encoder::encode_hex( const char *data, unsigned size )
{
	if ( size % 2 )
	{
		status_ = false;
	}
	if ( !status_ )
	{
		return std::string();
	}
	encoded_bytes_ += size / 2;
	unsigned pos = 0;
	std::string result;
	while( true )
	{
		for( ; n_ < 3 && pos < size; n_++, pos += 2 )
		{
			char byte;
			if ( !hex_byte( data[pos], data[pos + 1], byte ) )
			{
				status_ = false;
				return std::string();
			}
			chunk_[n_] = byte;
		}
		if ( n_ < 3 )
		{
			break;
		}
		const char *p = chunk_;
		if ( !process_chunk( p, result, 0 ) )
		{
			status_ = false;
			return std::string();
		}
		n_ = 0;
	}
	return result;
}

std::string Encoder::finalize()
{
	if ( !status_ )
	{
		return std::string();
	}
	std::string result;
	unsigned result_len = ( ( encoded_bytes_ * 8u ) / 6u ) + ( ( encoded_bytes_ * 8u ) % 6u ? 1 : 0 );
	unsigned padding_len = ( ( result_len + 3u ) & ~3u ) - result_len;

	if ( n_ )
	{
		for( unsigned i = n_; i < 3; i++ )
		{
			chunk_[i] = '\0';
		}
		const char *p = chunk_;
		if ( !process_chunk( p, result, 0 ) )
		{
			status_ = false;
			return std::string();
		}
		for( unsigned i = 0; i < padding_len; i++ )
		{
			result[result.size() - i - 1] = '=';
		}
	}
	return result;
}


Decoder::Decoder() :
	status_( true ),
	done_( false ),
	n_( 0 )
{
}

Decoder::operator bool() const
{
	return status_;
}

bool Decoder::done() const
{
	return done_ || n_ == 0;
}

Decoder& Decoder::reset()
{
	status_ = true;
	done_ = false;
	n_ = 0;
	return *this;
}

bool Decoder::decode( const char *data, unsigned size, std::vector<char> &out )
{
	return decode_( data, size, out, 1, &append );
}

bool Decoder::decode_hex( const char *data, unsigned size, std::vector<char> &out )
{
	return decode_( data, size, out, 2, &append_hex );
}

bool Decoder::decode_( const char *data, unsigned size, std::vector<char> &out, unsigned n, void (*append)( std::vector<char>&, char ) )
{
	if ( !status_ )
	{
		return false;
	}
	unsigned pos = 0;
	while( true )
	{
		for( ; n_ < 4 && pos < size; n_++, pos++ )
		{
			if ( valid_base64_characters_[(unsigned)data[pos]] || data[pos] == '=' )
			{
				chunk_[n_] = data[pos];
				continue;
			}
			status_ = false;
			break;
		}
		if ( n_ < 4 )
		{
			break;
		}
		if ( chunk_[0] == '=' || chunk_[1] == '=' )
		{
			status_ = false;
			break;
		}
		char buf[] = {
			(char)index_by_char( chunk_[0] ),
			(char)index_by_char( chunk_[1] ),
			(char)index_by_char( chunk_[2] ),
			(char)index_by_char( chunk_[3] )
		};
		append( out, ( buf[0] << 2 ) + ( ( buf[1] & 0x30 ) >> 4 ) );
		append( out, ( ( buf[1] & 0x0f ) << 4 ) + ( ( buf[2] & 0x3c ) >> 2 ) );
		append( out, ( ( buf[2] & 0x03 ) << 6 ) + ( buf[3] & 0x3f ) );
		n_ = 0;
		if ( chunk_[2] == '=' )
		{
			if ( chunk_[3] == '=' )
			{
				out.resize( out.size() - 2 * n );
				done_ = true;
				break;
			}
			else
			{
				status_ = false;
				break;
			}
		}
		else if ( chunk_[3] == '=' )
		{
			out.resize( out.size() - n );
			done_ = true;
			break;
		}
	}
	return status_;
}

} // namespace base64
