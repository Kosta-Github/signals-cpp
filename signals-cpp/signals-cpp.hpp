#pragma once

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
#  define SIGNALS_CPP_NOEXCEPT throw()
#else // defined(_MSC_VER) && (_MSC_VER <= 1800)
#  define SIGNALS_CPP_NOEXCEPT noexcept
#endif // defined(_MSC_VER) && (_MSC_VER <= 1800)

#if defined(_MSC_VER) && (_MSC_VER <= 1800)
#  define SIGNALS_CPP_NEED_EXPLICIT_MOVE
#endif // defined(_MSC_VER) && (_MSC_VER <= 1800)

#if !defined(_MSC_VER) || (_MSC_VER >= 1800)
#  define SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES
#endif // !defined(_MSC_VER) || (_MSC_VER >= 1800)

#include "connections.hpp"

namespace signals { }
