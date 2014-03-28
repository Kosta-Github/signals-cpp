// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"

CUTE_TEST("a passed check in a separate thread is detected", "pass") {
    auto t = cute::thread([&]() { CUTE_ASSERT(true); });
}

CUTE_TEST("a failed check in a separate thread is detected", "fail") {
    auto t = cute::thread([&]() { CUTE_ASSERT(false); });
}

CUTE_TEST("an unhandled exception in a cute::thread is detected", "fail") {
    auto t = cute::thread([&]() { throw std::runtime_error("forced exception"); });
}

CUTE_TEST("actions registered with 'at_tick()' are executed in the right order", "pass") {
    cute::tick ticker;
    std::atomic<int> value(0);

    auto t2 = cute::thread([&]() { ticker.at_tick(2, [&]() { CUTE_ASSERT(value == 2); ++value; CUTE_ASSERT(value == 3); }); });
    auto t1 = cute::thread([&]() { ticker.at_tick(1, [&]() { CUTE_ASSERT(value == 1); ++value; CUTE_ASSERT(value == 2); }); });
    auto t0 = cute::thread([&]() { ticker.at_tick(0, [&]() { CUTE_ASSERT(value == 0); ++value; CUTE_ASSERT(value == 1); }); });

    ticker.at_tick(3, [&]() { CUTE_ASSERT(value == 3); });
}

CUTE_TEST("actions registered with 'at_tick()' for the same tick are failing", "fail") {
    cute::tick ticker;

    ticker.at_tick(0, [&]() { });
    ticker.at_tick(0, [&]() { });
}

CUTE_TEST("actions registered with 'blocks_until_tick()' are passing if they are returning as scheduled", "pass") {
    cute::tick ticker;
    std::atomic<bool> proceed(false);

    auto t = cute::thread([&]() {
        ticker.reached_tick(0);
        ticker.at_tick(2, [&]() { proceed = true; });
    });

    ticker.reached_tick(1);
    ticker.blocks_until_tick(3, [&]() { while(!proceed) { } });
}

CUTE_TEST("actions registered with 'blocks_until_tick()' are failing if they are returning earlier", "fail") {
    cute::tick ticker;

    ticker.blocks_until_tick(1, [&]() { });
}
