#include <atomic>
#include <thread>

namespace signals {
    namespace test {

        struct milestone {
            milestone() : value(0) { }

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
