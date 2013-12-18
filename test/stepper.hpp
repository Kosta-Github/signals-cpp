//
// The MIT License (MIT)
//
// Copyright (c) 2013 by Konstantin (Kosta) Baumann & Autodesk Inc.
//
// Permission is hereby granted, free of charge,  to any person obtaining a copy of
// this software and  associated documentation  files  (the "Software"), to deal in
// the  Software  without  restriction,  including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software,  and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this  permission notice  shall be included in all
// copies or substantial portions of the Software.
//
// THE  SOFTWARE  IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE  LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
// IN  AN  ACTION  OF  CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#pragma once

#include <atomic>
#include <thread>

namespace signals {
    namespace test {

        struct stepper {
            stepper() : value(0) { }

            void reached(int v) {
                execute_at(v, []() { });
            }

            template<typename DELAY>
            void delay(int v, const DELAY& delay) {
                execute_at(v, [=]() { std::this_thread::sleep_for(delay); });
            }

            template<typename FUNC>
            void execute_at(int v, FUNC&& func) {
                while(value < v) { std::this_thread::yield(); }
                assert(value == v);
                func();
                assert(value == v);
                value = v + 1;
            }

            template<typename FUNC>
            bool returns_after(int v, FUNC&& func) {
                func();
                return (value > v);
            }

            std::atomic<int> value;
        };

    } // namespace test
} // namespace milestone
