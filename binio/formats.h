#if !defined(_H_BINIO_FORMATS_H_)
#define _H_BINIO_FORMATS_H_

#include "buffer.h"

namespace binio
{
    //=========================================================================================
    //write/read

    //Integers
    buffer& write_char(buffer& os, int8_t c);
    buffer& write_short(buffer& os, int16_t x);
    buffer& write_int(buffer& os, int32_t x);

    buffer& read_char(buffer& is, int8_t& c);
    buffer& read_short(buffer& is, int16_t& c);
    buffer& read_int(buffer& is, int32_t& c);

    //Unsigned integers
    buffer& write_byte(buffer& os, uint8_t c);
    buffer& write_ushort(buffer& os, uint16_t x);
    buffer& write_uint(buffer& os, uint32_t x);

    buffer& read_byte(buffer& is, uint8_t& c);
    buffer& read_ushort(buffer& is, uint16_t& c);
    buffer& read_ushort(buffer& is, uint32_t& c);

    //Floats
    buffer& write_float(buffer& os, float x);
    buffer& write_double(buffer& os, double x);

    buffer& read_float(buffer& is, float& x);
    buffer& read_double(buffer& is, double& x);

    //Strings
    buffer& write_string(buffer& os, const char* str);
    buffer& read_string(buffer& is, char* str);

    //=========================================================================================
    //operator<<

    //Integers
    buffer& operator<<(buffer& os, int8_t c);
    buffer& operator<<(buffer& os, int16_t x);
    buffer& operator<<(buffer& os, int32_t x);

    buffer& operator>>(buffer& is, int8_t& c);
    buffer& operator>>(buffer& is, int16_t& c);
    buffer& operator>>(buffer& is, int32_t& c);

    //Unsigned integers
    buffer& operator<<(buffer& os, uint8_t c);
    buffer& operator<<(buffer& os, uint16_t x);
    buffer& operator<<(buffer& os, uint32_t x);

    buffer& operator>>(buffer& is, uint8_t& c);
    buffer& operator>>(buffer& is, uint16_t& c);
    buffer& operator>>(buffer& is, uint32_t& c);

    //Floats
    buffer& operator<<(buffer& os, float x);
    buffer& operator<<(buffer& os, double x);

    buffer& operator>>(buffer& is, float& x);
    buffer& operator>>(buffer& is, double& x);

    //Strings
    buffer& operator<<(buffer& os, const char* str);
    buffer& operator>>(buffer& is, char* str);
};

#endif
