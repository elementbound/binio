#include "formats.h"
#include <cmath> //frexp
#include <cstring> //strlen

namespace binio
{
    //=============================================================================================
    //Signed integers
    //=============================================================================================

    buffer& write_char(buffer& os, int8_t c)
    {
        os.put(c);
        return os;
    };

    buffer& write_short(buffer& os, int16_t x)
    {
        os.write_element(&x,2);
        return os;
    };

    buffer& write_int(buffer& os, int32_t x)
    {
        os.write_element(&x,4);
        return os;
    };

    //

    buffer& read_char(buffer& is, int8_t& ret)
    {
        ret=is.get();
        return is;
    };

    buffer& read_short(buffer& is, int16_t& ret)
    {
        is.read_element(&ret, 2);
        return is;
    };

    buffer& read_int(buffer& is, int32_t& ret)
    {
        is.read_element(&ret, 4);
        return is;
    };

    //=============================================================================================
    //Unsigned integers
    //=============================================================================================

    buffer& write_byte(buffer& os, uint8_t c)
    {
        os.put(c);
        return os;
    };

    buffer& write_ushort(buffer& os, uint16_t x)
    {
        os.write_element(&x,2);
        return os;
    };

    buffer& write_uint(buffer& os, uint32_t x)
    {
        os.write_element(&x,4);
        return os;
    };

    //

    buffer& read_byte(buffer& is, uint8_t& ret)
    {
        ret=is.get();
        return is;
    };

    buffer& read_ushort(buffer& is, uint16_t& ret)
    {
        is.read_element(&ret,2);
        return is;
    };

    buffer& read_uint(buffer& is, uint32_t& ret)
    {
        is.read_element(&ret,4);
        return is;
    };

    //=============================================================================================
    //Floats
    //=============================================================================================

    buffer& write_float(buffer& os, float x)
    {
        uint32_t u;
        memcpy(&u,&x, 4);
        os.write_element(&u,4);
        return os;
    };

    buffer& write_double(buffer& os, double x)
    {
        uint64_t u;
        memcpy(&u,&x, 8);
        os.write_element(&u,8);
        return os;
    };

    //

    buffer& read_float(buffer& is, float& x)
    {
        uint32_t u;
        is.read_element(&u,4);
        memcpy(&x,&u,4);
        return is;
    };

    buffer& read_double(buffer& is, double& x)
    {
        uint64_t u;
        is.read_element(&u,8);
        memcpy(&x,&u,8);
        return is;
    };

    //=============================================================================================
    //Strings
    //=============================================================================================

    buffer& write_string(buffer& os, const char* str)
    {
        size_t len=strlen(str);
        os.write(str,len);
        os.put(0);

        return os;
    };

    buffer& read_string(buffer& is, char* str)
    {
        size_t len=0, start=is.tellg();
        while(is[start+len]!=0 && start+len<is.size()){len++;}
        is.read(str, len);

        return is;
    };

    //=============================================================================================
    //Aliases
    //=============================================================================================
    //Integers
    buffer& operator<<(buffer& os, int8_t c){return write_char(os,c);};
    buffer& operator<<(buffer& os, int16_t x){return write_short(os,x);};
    buffer& operator<<(buffer& os, int32_t x){return write_int(os,x);};

    buffer& operator>>(buffer& is, int8_t& c){return read_char(is,c);};
    buffer& operator>>(buffer& is, int16_t& c){return read_short(is,c);};
    buffer& operator>>(buffer& is, int32_t& c){return read_int(is,c);};

    //Unsigned integers
    buffer& operator<<(buffer& os, uint8_t c){return write_byte(os,c);};
    buffer& operator<<(buffer& os, uint16_t x){return write_ushort(os,x);};
    buffer& operator<<(buffer& os, uint32_t x){return write_uint(os,x);};

    buffer& operator>>(buffer& is, uint8_t& c){return read_byte(is,c);};
    buffer& operator>>(buffer& is, uint16_t& c){return read_ushort(is,c);};
    buffer& operator>>(buffer& is, uint32_t& c){return read_uint(is,c);};

    //Floats
    buffer& operator<<(buffer& os, float x){return write_float(os,x);};
    buffer& operator<<(buffer& os, double x){return write_double(os,x);};

    buffer& operator>>(buffer& is, float& x){return read_float(is,x);};
    buffer& operator>>(buffer& is, double& x){return read_double(is,x);};

    //Strings
    buffer& operator<<(buffer& os, const char* str){return write_string(os,str);};
    buffer& operator>>(buffer& is, char* str){return read_string(is,str);};
};
