#if !defined(__log_h__)
#define __log_h__

#include <stdio.h>

#ifndef NDEBUG

#define ERROR(x) (fprintf(stderr, "(ERROR) at %s:%d -> %s", __FILE__, __LINE__, (x)) , 0)
#define INFO(x)  printf("(LOG) %s", (x))

#else

#define ERROR(x)
#define INFO(x)

#endif

#endif // __log_h__
