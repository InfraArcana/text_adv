#include "init.hpp"

#include <stdarg.h>
#include <stdio.h>

#include "io.hpp"

#ifndef NDEBUG
void trace_util_func(const char* file, int line, const char* func, const char* fmt, ...)
{
    char msg_buffer[1000];
    va_list argptr;

    va_start(argptr, fmt);
    vsnprintf(msg_buffer, 1000, fmt, argptr);
    va_end(argptr);

    printf("\n%s, line %d, %s():\n%s\n", file, line, func, msg_buffer);
}
#endif // NDEBUG

namespace init
{

void init_io()
{
    TRACE_FUNC_BEGIN;
    io::init();
    TRACE_FUNC_END;
}

void cleanup_io()
{
    TRACE_FUNC_BEGIN;
    io::cleanup();
    TRACE_FUNC_END;
}

void init_game()
{
    TRACE_FUNC_BEGIN;

    TRACE_FUNC_END;
}

void cleanup_game()
{
    TRACE_FUNC_BEGIN;

    TRACE_FUNC_END;
}

void init_session()
{
    TRACE_FUNC_BEGIN;

    TRACE_FUNC_END;
}

void cleanup_session()
{
    TRACE_FUNC_BEGIN;

    TRACE_FUNC_END;
}

} // Init
