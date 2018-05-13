#pragma once
// stolen from https://github.com/gpakosz/Assert
#if defined(_WIN32)
#  define m_debug_break() __debugbreak()
#elif defined(__ORBIS__)
#  define m_debug_break() __builtin_trap()
#elif defined(__clang__)
#  define m_debug_break() __builtin_debugtrap()
#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__APPLE__)
#  include <signal.h>
#  define m_debug_break() raise(SIGTRAP)
#elif defined(__GNUC__)
#  define m_debug_break() __builtin_trap()
#else
#  define m_debug_break() ((void)0)
#endif

#define m_debug(fmt, ...) fprintf(stderr, "%s:%d: " fmt "\n", __func__, __LINE__, __VA_ARGS__)

inline void m_assert(bool x)
{
	if (!x) { m_debug_break(); }
}
