#pragma once

#if defined(WIN32)
#  if defined(signals_cpp_EXPORTS)
#    define SIGNALS_API __declspec(dllexport)
#  else // defined(signals_cpp_EXPORTS)
#    define SIGNALS_API __declspec(dllimport)
#    pragma comment(lib, "signals-cpp.lib")
#  endif // defined(signals_cpp_EXPORTS)
#else // defined(WIN32)
#  define SIGNALS_API
#endif // defined(WIN32)

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
#  define SIGNALS_CPP_NOEXCEPT throw()
#  define SIGNALS_CPP_NEED_EXPLICIT_MOVE
#else // defined(_MSC_VER) && (_MSC_VER <= 1800)
#  define SIGNALS_CPP_NOEXCEPT noexcept
#endif // defined(_MSC_VER) && (_MSC_VER <= 1800)

namespace signals { }
