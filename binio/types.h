#ifndef _H_BINIO_TYPES_H_
#define _H_BINIO_TYPES_H_

#ifdef BINIO_MANUALTYPES
    //These work only when compiling for x86 atm
    //Also, this solution is officially unsupported - take care!
    typedef int8_t  signed char;
    typedef int16_t signed short;
    typedef int32_t signed int;
    typedef int64_t signed long long;

    typedef uint8_t  unsigned char;
    typedef uint16_t unsigned short;
    typedef uint32_t unsigned int;
    typedef uint64_t unsigned long long;
#else
    #if __cplusplus > 199711L && !defined(BINIO_USE_STDINT)
        #include <cstdint>
    #else
        #include <stdint.h>
    #endif
#endif

#endif /* _H_BINIO_TYPES_H_ */
