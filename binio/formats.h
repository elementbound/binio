#if !defined(_H_BINIO_FORMATS_H_)
#define _H_BINIO_FORMATS_H_

#include "buffer.h"

namespace binio
{
    /** An important note **/
    //Every function here writes little-endian data!
    //Initially, the library's purpose was to write and read data written by itself, so
    //bi-endianness was not really a goal. This might actually change.

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

    buffer& operator>>(buffer& os, float& x);
    buffer& operator>>(buffer& os, double& x);

    //Strings
    buffer& operator<<(buffer& os, const char* str);
    buffer& operator>>(buffer& is, char* str);
};

#endif
