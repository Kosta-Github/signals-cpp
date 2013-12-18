signals-cpp[![Build Status](https://travis-ci.org/Kosta-Github/signals-cpp.png)](https://travis-ci.org/Kosta-Github/signals-cpp)
===========
Provide a very simple header-only C++ interface for creating and managing signal-slot-connections in an easy and thread-safe manner.

misc
====
This library is header-only and uses some new C++11 features (such as `std::mutex`, `std::atomic`, lambda-expressions, and ...) to provide an easy-to-use interface and being thread-safe at the same time. There are several important advantages over the approach used by the Qt signal-and-slot implementation:
- no need for an additional build step (i.e., `MOC` compiler),
- no special macros and language extensions required (i.e., `Q_EMIT`, `Q_SLOTS`, `Q_OBJECT`, ...),
- not intrusive, since no inheritance from a certain base class is required (i.e., `QObject`),
- checking whether the signal interface matches that of the target slot happens during compile-time, and
- free-standing functions and lambda-expressions can also be used as a target slots.
 
external dependencies
=====================
- [catch](https://github.com/philsquared/Catch) (only for unit tests)
