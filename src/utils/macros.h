
#ifndef MACROS_H
#define MACROS_H

#define DEBUG 0

#if DEBUG
#define DEBUG_MESSAGE(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_MESSAGE(fmt, ...)
#endif

#endif
