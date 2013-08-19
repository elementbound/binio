#include "buffer.h"
#include <cstring> //memcpy

namespace binio
{
    //=============================================================================================
    //Global init
    //=============================================================================================
    endian_t init(void)
    {
        unsigned short x=1;
        unsigned char* b=(unsigned char*)&x;
        if(b[0]==1){buffer::m_SysEndian=eLE;}
        else{buffer::m_SysEndian=eBE;}

        return buffer::m_SysEndian;
    };

    endian_t buffer::m_SysEndian;

    //=============================================================================================
    //Internal writers
    //=============================================================================================
    void buffer::write_raw(const byte_t* d, size_t s)
    {
        if(m_SeekW+s>m_Size){resize(m_SeekW+s);}

        for(size_t i=0; i<s; i++)
        {m_Data[m_SeekW+i]=*(d+i);}

        m_SeekW+=s;
    };

    void buffer::write_swapped(const byte_t* d, size_t s)
    {
        if(m_SeekW+s>m_Size){resize(m_SeekW+s);}

        for(size_t i=0; i<s; i++)
        {m_Data[m_SeekW+s-(i+1)]=*(d+i);}

        m_SeekW+=s;
    };

    //=============================================================================================
    //Internal readers
    //=============================================================================================
    size_t buffer::read_raw(byte_t* d, size_t s)
    {
        if(m_SeekR+s>m_Size){return 0;}
        for(size_t i=0; i<s; i++){d[i]=m_Data[m_SeekR+i];}

        m_SeekR+=s;
        return s;
    };

    size_t buffer::read_swapped(byte_t* d, size_t s)
    {
        if(m_SeekR+s>m_Size){return 0;}
        for(size_t i=0; i<s; i++){d[i]=m_Data[m_SeekR+s-(i+1)];}

        m_SeekR+=s;
        return s;
    };

    //=============================================================================================
    //Constructors, destructors
    //=============================================================================================

    buffer::buffer(void):
        m_Data(NULL),
        m_Size(0),
        m_Capacity(0),
        m_Bitbuffer(0),
        m_Bitindex(0),
        m_SeekR(0),
        m_SeekW(0),

        m_Endian(m_SysEndian),
        m_writeelem(&buffer::write_raw),
        m_readelem(&buffer::read_raw)
        {};

    buffer::buffer(const buffer& rhs)
    {
        m_Size=rhs.m_Size;
        m_Capacity=rhs.m_Capacity;
        m_Data=new byte_t[m_Capacity];
        memcpy(m_Data, rhs.m_Data,rhs.m_Size); //Copy existing data
        //memset(m_Data+m_Size, 0,m_Capacity-m_Size); //Zero the rest; Why?

        m_Bitbuffer=rhs.m_Bitbuffer;
        m_Bitindex=rhs.m_Bitindex;

        m_SeekR=rhs.m_SeekR;
        m_SeekW=rhs.m_SeekW;

        m_Endian=rhs.m_Endian;
        m_writeelem=rhs.m_writeelem;
        m_readelem=rhs.m_readelem;
    };

    buffer::~buffer(void)
    {
        free();
    };

    //=============================================================================================
    //Properties
    //=============================================================================================

    void* buffer::data(void) const
    {
        return m_Data;
    };

    size_t buffer::size(void) const
    {
        return m_Size;
    };

    size_t buffer::capacity(void) const
    {
        return m_Capacity;
    };

    //=============================================================================================
    //Endianness
    //=============================================================================================
    endian_t buffer::get_sys_endianness(void) const
    {
        return m_SysEndian;
    };

    endian_t buffer::get_endianness(void) const
    {
        return m_Endian;
    };

    void buffer::set_endianness(endian_t e)
    {
        m_Endian=e;

        if(m_Endian==m_SysEndian)
        {
            m_writeelem=&buffer::write_raw;
            m_readelem=&buffer::read_raw;
        }
        else
        {
            m_writeelem=&buffer::write_swapped;
            m_readelem=&buffer::read_swapped;
        }
    };

    endian_t buffer::swap_endianness(void)
    {
        if(m_Endian==eLE){m_Endian=eBE;}
        else{m_Endian=eLE;}

        if(m_Endian==m_SysEndian)
        {
            m_writeelem=&buffer::write_raw;
            m_readelem=&buffer::read_raw;
        }
        else
        {
            m_writeelem=&buffer::write_swapped;
            m_readelem=&buffer::read_swapped;
        }

        return m_Endian;
    };

    //=============================================================================================
    //Modifiers
    //=============================================================================================

    void buffer::free(void)
    {
        delete [] m_Data;

        m_Data=NULL;
        m_Size=0;
        m_Capacity=0;

        m_Bitbuffer=0;
        m_Bitindex=0;

        m_SeekR=0;
        m_SeekW=0;
    };

    void buffer::resize(size_t new_size)
    {
        if(new_size==0)
        {
            free();
            return;
        }

        if(new_size<=m_Capacity)
        {
            //We still have space, so we just have to acknowledge the change
            m_Size=new_size;
        }
        else
        {
            //Determine new capacity
            //(you might want to use something more complex)
            m_Capacity=(new_size/512+1)*512;

            byte_t* new_data=new byte_t[m_Capacity];
            if(m_Data!=NULL)
            {
                memcpy(new_data, m_Data,m_Size);
                delete [] m_Data;
            }
            m_Data=new_data;

            m_Size=new_size;
        }

        if(m_SeekR>=m_Size){m_SeekR=m_Size-1;}
        if(m_SeekW>=m_Size){m_SeekW=m_Size-1;}
    };

    //=============================================================================================
    //Stack - push
    //=============================================================================================

    void buffer::push(byte_t byte)
    {
        resize(m_Size+1);
        m_Data[m_Size-1]=byte;
    };

    void buffer::push(void* data, size_t size)
    {
        resize(m_Size+size);
        memcpy(m_Data+m_Size-size, data, size);
    };

    //=============================================================================================
    //Stack - pop
    //=============================================================================================

    byte_t buffer::pop(void)
    {
        if(m_Size==0)
            {return 0;}

        if(m_Size==1)
        {
            byte_t r=m_Data[0];
            resize(0); //This is equal to calling free()
            return r;
        }
        else
        {
            //Note: this one won't work if you zero the rest after resizing
            resize(m_Size-1);
            return m_Data[m_Size];
        }
    };

    //Please note: the bytes are not in reverse order.
    //This is not the same as repeat(n){buff.pop();}
    size_t buffer::pop(void* data, size_t size)
    {
        if(size>m_Size){size=m_Size;}

        memcpy(data,m_Data,size);
        resize(m_Size-size);
        return size;
    };

    //=============================================================================================
    //Pushing bits
    //=============================================================================================
    //Bit-pushing is quite limited. Bits are *actually* pushed when you have a whole byte waiting,
    //or when you flush. Until then, they are kept in their own little buffer.

    void buffer::push_bit(bool val)
    {
        m_Bitbuffer|=val<<m_Bitindex;
        m_Bitindex++;
        if(m_Bitindex==8){flush_bits();}
    };

    void buffer::flush_bits(void)
    {
        if(m_Bitindex==0){return;}
        push(m_Bitbuffer);
        m_Bitindex=0;
        m_Bitbuffer=0;
    };

    //=============================================================================================
    //fstream-like stuff
    //=============================================================================================

    size_t buffer::read(void* data, size_t size)
    {
        if(m_SeekR+size>=m_Size){size=m_Size-m_SeekR;}

        memcpy(data, m_Data+m_SeekR,size);
        m_SeekR+=size;
        return size;
    };

    void buffer::write(const void* data, size_t size)
    {
        if(m_SeekW+size>=m_Size){resize(m_SeekW+size);}

        memcpy(m_Data+m_SeekW, data,size);
        m_SeekW+=size;
    };

    void buffer::seekp(int i, bool rel)
    {
        m_SeekW=m_SeekW*rel+i;
    };

    void buffer::seekg(int i, bool rel)
    {
        m_SeekR=m_SeekR*rel+i;
    };

    size_t buffer::tellp(void) const
    {
        return m_SeekW;
    };

    size_t buffer::tellg(void) const
    {
        return m_SeekR;
    };

    void buffer::put(byte_t byte)
    {
        if(m_SeekW==m_Size){resize(m_Size+1);}
        m_Data[m_SeekW++]=byte;
    };

    byte_t buffer::get(void)
    {
        if(m_SeekR>=m_Size){return 0;}
        return m_Data[m_SeekR++];
    };

    //=============================================================================================
    //Endian-correct I/O
    //=============================================================================================
    void buffer::write_element(const void* d, size_t s)
    {
        (this->*m_writeelem)((const byte_t*)d,s);
    };

    size_t buffer::read_element(void* d, size_t s)
    {
        return (this->*m_readelem)((byte_t*)d,s);
    };

    //=============================================================================================
    //Getters
    //=============================================================================================

    byte_t& buffer::byte(size_t i)
    {
        //What if size is zero?
        return m_Data[i];
    };

    const byte_t& buffer::byte(size_t i) const
    {
        //What if size is zero?
        return m_Data[i];
    };

    byte_t& buffer::operator[](size_t i) {return byte(i);}
    const byte_t& buffer::operator[](size_t i) const {return byte(i);}

    //=============================================================================================
    //Bithandling
    //=============================================================================================


    void buffer::set_bit(size_t i, bool v)
    {
        size_t byte_index, bit_index;
        byte_index=i/8;
        bit_index=i%8;

        if(byte_index>=size()){return;}
        if(v){m_Data[byte_index]|=1<<bit_index;}
        else{m_Data[byte_index]&=~(1<<bit_index);}
    };

    bool buffer::get_bit(size_t i) const
    {
        size_t byte_index, bit_index;
        byte_index=i/8;
        bit_index=i%8;

        if(byte_index>=size()){return 0;}
        return ((byte(byte_index)>>bit_index)&1)!=0;
    };
};
