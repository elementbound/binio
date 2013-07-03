#include "buffer.h"
#include <cstring> //memcpy

namespace binio
{
    //=============================================================================================
    //Constructors, destructors
    //=============================================================================================

    buffer::buffer(void):
        m_Data(NULL),
        m_Size(0),
        m_Capacity(0),
        m_Bitbuffer(0),
        m_Bitindex(0),
        m_Seek(0)
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

        m_Seek=rhs.m_Seek;
    };

    buffer::~buffer(void)
    {
        free();
    };

    //=============================================================================================
    //Getters/properties
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

        m_Seek=0;
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

        if(m_Seek>=m_Size){m_Seek=m_Size-1;}
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
        if(m_Seek+size>=m_Size){size=m_Size-m_Seek;}

        memcpy(data, m_Data+m_Seek,size);
        m_Seek+=size;
        return size;
    };

    void buffer::write(const void* data, size_t size)
    {
        if(m_Seek+size>=m_Size){resize(m_Seek+size);}

        memcpy(m_Data+m_Seek, data,size);
        m_Seek+=size;
    };

    void buffer::seek(size_t i)
    {
        m_Seek=i;
    };

    void buffer::seek_rel(int i)
    {
        m_Seek+=i;
    };

    size_t buffer::tell(void) const
    {
        return m_Seek;
    };

    void buffer::put(byte_t byte)
    {
        if(m_Seek==m_Size){resize(m_Size+1);}
        m_Data[m_Seek++]=byte;
    };

    byte_t buffer::get(void)
    {
        if(m_Seek>=m_Size){return 0;}
        return m_Data[m_Seek++];
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
