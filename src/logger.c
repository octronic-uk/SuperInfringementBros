#include "logger.h"

static int _debug_ = 0;
static int _error_ = 1;

#include <stdarg.h>
#include <stdio.h>

void debug(char* format, ...) {
    if (_debug_) {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stdout, format, argptr);
        va_end(argptr);
    }
}

void error(char* format, ...) {
    if (_error_) {
        va_list argptr;
        va_start(argptr, format);
        vfprintf(stderr, format, argptr);
        va_end(argptr);
    }
}

