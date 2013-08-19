#if !defined(_H_BINIO_BUFFER_H_)
#define _H_BINIO_BUFFER_H_
#include <cstddef> //size_t
#include "types.h"

namespace binio
{
    typedef uint8_t byte_t;

    typedef enum
    {
        eLE,
        eBE
    } endian_t;

    endian_t init(void);

    class buffer
    {
        private:
            byte_t* m_Data;
            size_t m_Size;
            size_t m_Capacity;

            byte_t m_Bitbuffer;
            byte_t m_Bitindex;

            size_t m_SeekR;
            size_t m_SeekW;

            static endian_t m_SysEndian;
            endian_t m_Endian;

            //Internal writers
            void write_raw(const byte_t*, size_t);
            void write_swapped(const byte_t*, size_t);

            //Internal readers
            size_t read_raw(byte_t*, size_t);
            size_t read_swapped(byte_t*, size_t);

            void(buffer::*m_writeelem)(const byte_t*, size_t);
            size_t(buffer::*m_readelem)(byte_t*, size_t);

        public:
            buffer(void);
            buffer(const buffer&);
            ~buffer(void);

            //Properties
            void* data(void) const;
            size_t size(void) const;
            size_t capacity(void) const;

            //Endianness
            endian_t get_sys_endianness(void) const;
            endian_t get_endianness(void) const;
            void     set_endianness(endian_t);
            endian_t swap_endianness(void);

            //Modifiers
            void free(void);
            void resize(size_t new_size);

            //Stack
            void push(byte_t byte);
            void push(void* data, size_t size);

            byte_t pop(void);
            size_t pop(void* data, size_t size);

            //Pushing bits
            void push_bit(bool bit);
            void flush_bits(void);

            //fstream-like
            size_t read(void* data, size_t size);
            void   write(const void* data, size_t size);
            void   seekp(int i, bool rel=0);
            void   seekg(int i, bool rel=0);
            size_t tellp(void) const;
            size_t tellg(void) const;

            //Endian-correct I/O
            void   write_element(const void* d, size_t s);
            size_t read_element(void* d, size_t s);

            void put(byte_t byte);
            byte_t get(void);

            //Getters
            byte_t& operator[](size_t i);
            byte_t& byte(size_t i);
            const byte_t& operator[](size_t i) const;
            const byte_t& byte(size_t i) const;

            //Bithandling
            bool get_bit(size_t i) const;
            void set_bit(size_t i, bool v);

            friend endian_t init(void);
    };
};

#endif
