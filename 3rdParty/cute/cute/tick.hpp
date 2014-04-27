// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "macros.hpp"
#include "detail/decomposer.hpp"

#include <condition_variable>
#include <thread>

namespace cute {

    struct tick {
        inline tick(int start_tick_ = 0) : m_tick(start_tick_) { }

        template<typename FUNC>
        inline void at_tick(int tick_, FUNC&& func_) {
            {   // limit the scope of the lock to not include
                // the notify_all() call below; otherwise the
                // notified threads would still be blocked by
                // the locked mutex and go for a nap again

                // wait until the scheduled tick
                std::unique_lock<std::mutex> lock(m_mutex);
                m_wakeup.wait(lock, [&]() { return (m_tick >= tick_); });

                // verify that the tick value is correct (e.g., no
                // two actions have been registered for the same tick)
                CUTE_ASSERT(m_tick == tick_, CUTE_CAPTURE("at_tick(): several actions registered for the same tick"));

                // update the current tick
                ++m_tick;

                // perform the action
                func_();
            }

            // wakeup other waiting threads again to let them check
            // if they are due based on their scheduled tick
            m_wakeup.notify_all();
        }

        inline void reached_tick(int tick_) {
            // just a no-op in order to increase the current tick value from
            // a specific thread
            at_tick(tick_, []() { });
        }

        template<typename DELAY>
        inline void delay_tick_for(int tick_, DELAY&& delay_) {
            // just schedule a sleep operation for the given tick
            at_tick(tick_, [=]() { std::this_thread::sleep_for(delay_); });
        }
        
        template<typename FUNC>
        inline void blocks_until_tick(int tick_, FUNC&& func_) {
            // perform the action and check afterwards that
            // the action didn't return too early
            func_();

            CUTE_ASSERT(m_tick >= tick_, CUTE_CAPTURE("blocks_until_tick(): function returned too early"));
        }

    private:
        std::mutex m_mutex;
        std::condition_variable m_wakeup;
        std::atomic<int> m_tick;
    };

} // namespace cute
