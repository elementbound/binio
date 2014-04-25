#if !defined(_H_BINIO_FORMATS_H_)
#define _H_BINIO_FORMATS_H_

#include "buffer.h"
#include <fstream>

namespace binio
{
    //=========================================================================================
    //write/read

    //Integers
    buffer& write_char(buffer& os, char c);
    buffer& write_short(buffer& os, short x);
    buffer& write_int(buffer& os, int x);

    buffer& read_char(buffer& is, char& c);
    buffer& read_short(buffer& is, short& c);
    buffer& read_int(buffer& is, int& c);

    //Unsigned integers
    buffer& write_byte(buffer& os, unsigned char c);
    buffer& write_ushort(buffer& os, unsigned short x);
    buffer& write_uint(buffer& os, unsigned int x);

    buffer& read_byte(buffer& is, unsigned char& c);
    buffer& read_ushort(buffer& is, unsigned short& c);
    buffer& read_ushort(buffer& is, unsigned int& c);

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
    buffer& operator<<(buffer& os, char c);
    buffer& operator<<(buffer& os, short x);
    buffer& operator<<(buffer& os, int x);

    buffer& operator>>(buffer& is, char& c);
    buffer& operator>>(buffer& is, short& c);
    buffer& operator>>(buffer& is, int& c);

    //Unsigned integers
    buffer& operator<<(buffer& os, unsigned char c);
    buffer& operator<<(buffer& os, unsigned short x);
    buffer& operator<<(buffer& os, unsigned int x);

    buffer& operator>>(buffer& is, unsigned char& c);
    buffer& operator>>(buffer& is, unsigned short& c);
    buffer& operator>>(buffer& is, unsigned int& c);

    //Floats
    buffer& operator<<(buffer& os, float x);
    buffer& operator<<(buffer& os, double x);

    buffer& operator>>(buffer& is, float& x);
    buffer& operator>>(buffer& is, double& x);

    //Strings
    buffer& operator<<(buffer& os, const char* str);
    buffer& operator>>(buffer& is, char* str);

    //fstream
    std::ostream& operator<<(std::ostream& os, binio::buffer& b);
}

#endif
