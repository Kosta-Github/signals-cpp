// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <cassert>
#include <thread>

namespace cute {

    struct thread {
        template<typename FUNC>
        inline thread(FUNC&& func) : m_thread(wrap_func(std::forward<FUNC>(func))) { }
        inline thread(thread&& o) CUTE_NOEXCEPT : m_thread(std::move(o.m_thread)) { }
        inline thread& operator=(thread&& o) CUTE_NOEXCEPT { m_thread = std::move(o.m_thread); return *this; }
        inline ~thread() { if(m_thread.joinable()) { m_thread.join(); } }

        inline void join() { m_thread.join(); }

    private:
        template<typename FUNC>
        inline static std::function<void()> wrap_func(FUNC func) {
            auto test = cute::detail::eval_context::current().current_test;
            assert(test);

            auto wrapped = [=]() {
                try {
                    CUTE_DETAIL_ASSERT(
                        (static_cast<void>(func()), true), test->file, test->line,
                        "unhandled exception in separate thread detected",
                        cute::captures(), cute::captures()
                    );
                } catch(...) {
                }
            };

            return wrapped;
        }

        std::thread m_thread;
    };

} // namespace cute
