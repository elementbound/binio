#if !defined(_H_BINIO_BUFFER_H_)
#define _H_BINIO_BUFFER_H_
#include <cstddef> //size_t

namespace binio
{
    typedef unsigned char byte_t;

    class buffer
    {
        private:
            byte_t* m_Data;
            size_t m_Size;
            size_t m_Capacity;

            byte_t m_Bitbuffer;
            byte_t m_Bitindex;

            size_t m_Seek;

        public:
            buffer(void);
            buffer(const buffer&);
            ~buffer(void);

            void* data(void) const;
            size_t size(void) const;
            size_t capacity(void) const;

            void free(void);
            void resize(size_t new_size);

            void push(byte_t byte);
            void push(void* data, size_t size);

            void push_bit(bool bit);
            void flush_bits(void);

            byte_t pop(void);
            size_t pop(void* data, size_t size);

            size_t read(void* data, size_t size);
            void   write(const void* data, size_t size);
            void   seek(size_t i);
            void   seek_rel(int i);
            size_t tell(void) const;

            void put(byte_t byte);
            byte_t get(void);

            byte_t& operator[](size_t i);
            byte_t& byte(size_t i);
            const byte_t& operator[](size_t i) const;
            const byte_t& byte(size_t i) const;

            bool get_bit(size_t i) const;
            void set_bit(size_t i, bool v);
    };
};

#endif
