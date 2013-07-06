#include "formats.h"
#include <cmath> //frexp
#include <cstring> //strlen
//#include <iostream> //debug

namespace binio
{
    //=============================================================================================
    //Signed integers
    //=============================================================================================

    buffer& operator<<(buffer& os, char c)
    {
        if(os.size()<os.tell()+1){os.resize(os.size()+1);}
        size_t offs=os.tell()*8;

        if(c<0){c=-(c--); c=~c;}
        for(byte_t i=0; i<8; i++){os.set_bit(offs+i,(c>>i)&1);}

        os.seek_rel(1);
        return os;
    };

    buffer& operator<<(buffer& os, short x)
    {
        if(os.size()<os.tell()+2){os.resize(os.size()+2);}
        size_t offs=os.tell()*8;

        if(x<0){x=-(x--); x=~x;}
        for(byte_t i=0; i<16; i++){os.set_bit(offs+i,(x>>i)&1);}

        os.seek_rel(2);
        return os;
    };

    buffer& operator<<(buffer& os, int x)
    {
        if(os.size()<os.tell()+4){os.resize(os.size()+4);}
        size_t offs=os.tell()*8;

        if(x<0){x=-(x--); x=~x;}
        for(byte_t i=0; i<32; i++){os.set_bit(offs+i,(x>>i)&1);}

        os.seek_rel(4);
        return os;
    };

    //

    buffer& operator>>(buffer& is, char& ret)
    {
        if(is.size()<is.tell()+1){return is;}

        size_t offs=is.tell()*8;
        ret=0;
        for(size_t i=0; i<7; i++){ret<<=1; ret+=is.get_bit(offs+6-i);}
        if(is.get_bit(offs+7)){ret-=1<<7;}
        is.seek_rel(1);

        return is;
    };

    buffer& operator>>(buffer& is, short& ret)
    {
        if(is.size()<is.tell()+2){return is;}

        size_t offs=is.tell()*8;
        ret=0;
        for(size_t i=0; i<15; i++){ret<<=1; ret+=is.get_bit(offs+14-i);}
        if(is.get_bit(offs+15)){ret-=1<<15; ret+=1; /* wtf? */}
        is.seek_rel(2);

        return is;
    };

    buffer&   operator>>(buffer& is, int& ret)
    {
        if(is.size()<is.tell()+4){return is;}

        size_t offs=is.tell()*8;
        ret=0;
        for(size_t i=0; i<31; i++){ret<<=1; ret+=is.get_bit(offs+30-i);}
        if(is.get_bit(offs+31)){ret-=1<<31;}
        is.seek_rel(4);

        return is;
    };

    //=============================================================================================
    //Unsigned integers
    //=============================================================================================

    buffer& operator<<(buffer& os, unsigned char c)
    {
        if(os.size()<os.tell()+1){os.resize(os.size()+1);}
        size_t offs=os.tell()*8;

        for(byte_t i=0; i<8; i++){os.set_bit(offs+i,(c>>i)&1);}

        os.seek_rel(1);
        return os;
    };

    buffer& operator<<(buffer& os, unsigned short x)
    {
        if(os.size()<os.tell()+1){os.resize(os.size()+2);}
        size_t offs=os.tell()*8;

        for(byte_t i=0; i<16; i++){os.set_bit(offs+i,(x>>i)&1);}

        os.seek_rel(2);
        return os;
    };

    buffer& operator<<(buffer& os, unsigned int x)
    {
        if(os.size()<os.tell()+1){os.resize(os.size()+4);}
        size_t offs=os.tell()*8;

        for(byte_t i=0; i<32; i++){os.set_bit(offs+i,(x>>i)&1);}

        os.seek_rel(4);
        return os;
    };

    //

    buffer& operator>>(buffer& is, unsigned char& ret)
    {
        if(is.size()<is.tell()+1){return is;}

        size_t offs=is.tell()*8;
        ret=0;
        for(size_t i=0; i<8; i++){ret<<=1; ret+=is.get_bit(offs+7-i);}
        is.seek_rel(1);

        return is;
    };

    buffer& operator>>(buffer& is, unsigned short& ret)
    {
        if(is.size()<is.tell()+2){return is;}

        size_t offs=is.tell()*8;
        ret=0;
        for(size_t i=0; i<16; i++){ret<<=1; ret+=is.get_bit(offs+15-i);}
        is.seek_rel(2);

        return is;
    };

    buffer& operator>>(buffer& is, unsigned int& ret)
    {
        if(is.size()<is.tell()+4){return is;}

        size_t offs=is.tell()*8;
        ret=0;
        for(size_t i=0; i<32; i++){ret<<=1; ret+=is.get_bit(offs+31-i);}
        is.seek_rel(4);

        return is;
    };

    //=============================================================================================
    //Floats
    //=============================================================================================

    buffer& operator<<(buffer& os, float x)
    {
        using namespace std;

        int exponent;
        float significand=frexp(x, &exponent);
        if(significand<0.0f){significand=-significand;}

        size_t startbit=os.tell()*8;
        if(os.size()<os.tell()+4){os.resize(os.size()+4);}

        //Mantissa ( significand )
        for(size_t i=0; i<23; i++)
        {
            os.set_bit(startbit+22-i, significand>=0.5f);
            if(significand>=0.5f){significand-=0.5f;}
            significand*=2.0f;
        }

        //Exponent
        exponent+=(1<<7)-1;
        for(size_t i=0; i<8; i++){os.set_bit(startbit+23+i, (exponent>>i)&1);}

        //Sign bit
        os.set_bit(startbit+31, x<0.0);
        os.seek_rel(4);
        return os;
    };

    buffer& operator<<(buffer& os, double x)
    {
        int exponent;
        double significand=frexp(x, &exponent);
        if(significand<0.0){significand=-significand;}

        size_t startbit=os.tell()*8;
        if(os.size()<os.tell()+8){os.resize(os.size()+8);}

        //Mantissa ( significand )
        for(size_t i=0; i<52; i++)
        {
            os.set_bit(startbit+51-i, significand>=0.5);
            if(significand>=0.5){significand-=0.5;}
            significand*=2.0;
        }

        //Exponent
        exponent+=(1<<10)-1;
        for(size_t i=0; i<11; i++){os.set_bit(startbit+52+i, (exponent>>i)&1);}

        //Sign bit
        os.set_bit(startbit+63, x<0.0);
        os.seek_rel(8);
        return os;
    };

    //

    buffer& operator>>(buffer& is, float& x)
    {
        if(is.size()<is.tell()+4){return is;}

        size_t offs=is.tell()*8;
        int exponent=0; float significand=0.0f;

        //Get the mantissa ( significand )
        for(size_t i=0; i<23; i++){significand=(significand+is.get_bit(offs+i))/2.0f;}

        //Exponent
        for(size_t i=0; i<8; i++){exponent+=(is.get_bit(offs+23+i))<<i;}
        exponent-=(1<<7)-1;

        x=significand*pow(2.0f,exponent)*(is.get_bit(offs+31)?-1.0f:1.0f);
        is.seek_rel(4);
        return is;
    };

    buffer& operator>>(buffer& is, double& x)
    {
        if(is.size()<is.tell()+8){return is;}

        size_t offs=is.tell()*8;
        int exponent=0; double significand=0.0;

        //Get the mantissa ( significand )
        for(size_t i=0; i<52; i++){significand=(significand+is.get_bit(offs+i))/2.0;}

        //Exponent
        for(size_t i=0; i<11; i++){exponent+=(is.get_bit(offs+52+i))<<i;}
        exponent-=(1<<10)-1;

        x=significand*pow(2.0f,exponent)*(is.get_bit(offs+63)?-1.0f:1.0f);
        is.seek_rel(8);
        return is;
    };

    //=============================================================================================
    //Strings
    //=============================================================================================

    buffer& operator<<(buffer& os, const char* str)
    {
        size_t len=strlen(str);
        os.write(str,len);
        os.put(0);

        return os;
    };

    buffer& operator>>(buffer& is, char* str)
    {
        size_t len=0, start=is.tell();
        while(is[start+len]!=0 && start+len<is.size()){len++;}
        is.read(str, len);
        //str[len]='\0';

        is.seek_rel(len);
        return is;
    };
};
