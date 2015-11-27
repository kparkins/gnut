//
//  Macros.h
//  Graphics
//

#ifndef GFX_MACROS_H
#define GFX_MACROS_H

#if defined (__APPLE__) || defined(__MACH__) || defined(__GNUC__)

#define FORCE_INLINE inline __attribute__ ((always_inline))
#define ALIGN_16 __attribute__ ((aligned(16)))
#define __FILENAME__ (strrchr(__FILE__, '/') ? (char*) strrchr(__FILE__, '/') + 1 : __FILE__)

#elif defined(_WIN32)

#define FORCE_INLINE inline __forceinline
#define ALIGN_16 __declspec(align(16))
#define __FILENAME__ (strrchr(__FILE__, '\\') ? (char*) strrchr(__FILE__, '\\') + 1 : __FILE__)

#endif

#endif /* Macros_h */
