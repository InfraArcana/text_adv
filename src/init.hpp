#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#include <assert.h>

#ifdef NDEBUG

#define TRACE(args)
#define TRACE_VERBOSE(args)
#define TRACE_FUNC_BEGIN
#define TRACE_FUNC_BEGIN_VERBOSE
#define TRACE_FUNC_END
#define TRACE_FUNC_END_VERBOSE

#else // NDEBUG not defined

//-----------------------------------------------------------------------------
// OPTIONS
//-----------------------------------------------------------------------------
// Lvl of TRACE output in debug mode
// 0 : Disabled
// 1 : Standard
// 2 : Verbose
#define TRACE_LVL 1
//-----------------------------------------------------------------------------

#ifndef NDEBUG
bool trace_util_func(const char* file, int line, const char* func, const char* fmt, ...);
#endif // NDEBUG

//#define STR_HELPER(x) #x
//#define STR(x) STR_HELPER(x)

// Print specific trace message
#define TRACE(...) if (TRACE_LVL > 0) \
  trace_util_func(__FILE__, __LINE__, __func__, __VA_ARGS__)

// Print trace message if verbose mode is enabled
#define TRACE_VERBOSE(...) if (TRACE_LVL > 1) \
  trace_util_func(__FILE__, __LINE__, __func__, __VA_ARGS__)

// Print trace message for function start
#define TRACE_FUNC_BEGIN if (TRACE_LVL > 0) \
  printf("\n%s, line %d, %s()...\n", \
         __FILE__, __LINE__, __func__)

// Print trace message for function start if verbose mode is enabled
#define TRACE_FUNC_BEGIN_VERBOSE if (TRACE_LVL > 1) TRACE_FUNC_BEGIN

// Print trace message for function end
#define TRACE_FUNC_END if (TRACE_LVL > 0) \
  printf("\n%s, line %d, %s() [DONE]\n", \
         __FILE__, __LINE__, __func__)

// Print trace message for function end if verbose mode is enabled
#define TRACE_FUNC_END_VERBOSE if (TRACE_LVL > 1) TRACE_FUNC_END

#endif // NDEBUG

namespace init
{

void init_io();
void cleanup_io();

void init_game();
void cleanup_game();

void init_session();
void cleanup_session();

} // init

#endif // INIT_H
