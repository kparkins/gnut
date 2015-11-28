//
//  Macros.h
//  Graphics
//

#ifndef GNUT_MACROS_H
#define GNUT_MACROS_H

#if defined (__APPLE__) || defined(__MACH__)
    #define __FILENAME__ (strrchr(__FILE__, '/') ? (char*) strrchr(__FILE__, '/') + 1 : __FILE__)
#elif defined(_WIN32)
    #define __FILENAME__ (strrchr(__FILE__, '\\') ? (char*) strrchr(__FILE__, '\\') + 1 : __FILE__)
#endif

#if defined(_MSC_VER)
    #define FORCE_INLINE inline __forceinline
    #define ALIGN_16 __declspec(align(16))
#elif defined(__GNUC__)
    #define FORCE_INLINE inline __attribute__ ((always_inline))
    #define ALIGN_16 __attribute__ ((aligned(16)))
#endif

#endif /* GNUT_MACROS_H */
