// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <algorithm>
#include <cassert>
#include <functional>

#if defined(CUTE_HAS_NOEXCEPT)
#   define CUTE_NOEXCEPT noexcept
#else // defined(CUTE_HAS_NOEXCEPT)
#   define CUTE_NOEXCEPT throw()
#endif // defined(CUTE_HAS_NOEXCEPT)

namespace cute {

    /// The cleanup_guard helper class (also known as scope_guard) uses the RAII principle for
    /// ensuring that custom cleanup code is run even in cases of exceptions and early returns,
    /// while it reduces the coding complexity for doing so at the same time. This is achieved
    /// by creating a cleanup_guard object and passing in a lambda function to the constructor
    /// defining the custom cleanupaction that is required for cleaning up a certain resource
    /// gracefully in case of failures. As soon as the cleanup_guard object gets destructed
    /// (at the end of its scope) the cleanup function gets called and the specified action is
    /// executed. In cases where you want to dismiss the cleanup action (e.g., because all
    /// operations on the resource were successful and you want to keep it) you can call
    /// dismiss() on the cleanup_guard object and the cleanup action will not be triggered
    /// anymore. This comes in especially useful if you need to deal with several dependent
    /// resources and you want to maintain some "transactional" behavior such as keep them all
    /// or none of them.
    ///
    /// Example:
    ///     void func() {
    ///         const char* const filename = "my file.txt";
    ///         std::ofstream file(filename);
    ///         if(!file.is_open()) { return false; }
    ///
    ///         cleanup_guard cleanup([&] {
    ///             file.close(); remove(filename);
    ///         });
    ///
    ///         if(!write_to_file(file)) { return false; } // cleanup called
    ///
    ///         if(!write_to_file(file)) { throw ...; } // cleanup called
    ///
    ///         if(!file.good()) { return false; } // cleanup called
    ///
    ///         // all ops were successful => keep the file
    ///         cleanup.dismiss();
    ///         return true;
    ///     }
    ///
    class cleanup_guard {
    public:
        /// Constructs a new cleanup_guard object by passing in a cleanup
        /// callback function. The passed in callback can be a nullptr or
        /// it needs to be convertible to std::function<void()> and be
        /// callable for the whole lifetime of the constructed cleanup_guard
        /// object or at least up to the point where dismiss() or cleanup()
        /// is called.
        template<typename FUNC>
        inline explicit cleanup_guard(FUNC&& func) : m_func(std::forward<FUNC>(func)) { }

        // support move semantics
        inline cleanup_guard(cleanup_guard&& other) CUTE_NOEXCEPT { operator=(std::move(other)); }
        inline cleanup_guard& operator=(cleanup_guard&& other) CUTE_NOEXCEPT {
            assert(!m_func); std::swap(m_func, other.m_func); return *this;
        }

        /// Destructs the cleanup_guard and executes the cleanup callback
        /// if it hasn't been dismissed before.
        inline ~cleanup_guard() { cleanup(); }

        /// Dismisses the cleanup callback function so that it gets not called
        /// in the destructor.
        inline void dismiss() { m_func = nullptr; }

        /// Executes the cleanup callback function explicitly and earlier than
        /// on scope exit.
        inline void cleanup() { if(m_func) { m_func(); m_func = nullptr; } }

    private:
        // disable some auto-generated members
        cleanup_guard(); // = delete;
        cleanup_guard(const cleanup_guard& other); // = delete;
        cleanup_guard& operator=(const cleanup_guard& other); // = delete;

    private:
        std::function<void()> m_func;
    };

} // cute
