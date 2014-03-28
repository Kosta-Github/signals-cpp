// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"
#include "../cute/cleanup_guard.hpp"

CUTE_TEST("create a cleanup_guard without a callback/cleanup action", "pass") {
    auto cleanup = cute::cleanup_guard(nullptr);
    CUTE_ASSERT(true);
}

CUTE_TEST("create a cleanup_guard and execute the action at scope exit", "pass") {
    auto value = int(0);

    {
        auto cleanup = cute::cleanup_guard([&] { value = 42; });
    }

    CUTE_ASSERT(value == 42);
}

CUTE_TEST("create a cleanup_guard and execute the action in case of an exception at scope exit", "pass") {
    auto value = int(0);
    auto const exception_message = std::string("forced exception");

    try {
        auto cleanup = cute::cleanup_guard([&] { value = 42; });
        throw std::runtime_error(exception_message);
        cleanup.dismiss();
    } catch(std::runtime_error const& ex) {
        // ensure that only the expected exception has been thrown
        CUTE_ASSERT(ex.what() == exception_message);
    }

    CUTE_ASSERT(value == 42);
}

CUTE_TEST("create a cleanup_guard and do not execute after calling dismiss()", "pass") {
    auto value = int(0);

    {
        auto cleanup = cute::cleanup_guard([&] { value = 42; });
        cleanup.dismiss();
    }

    CUTE_ASSERT(value == 0);
}

CUTE_TEST("create a cleanup_guard and execute the cleanup action explicitly", "pass") {
    auto value = int(0);

    {
        auto cleanup = cute::cleanup_guard([&] { value = 42; });
        CUTE_ASSERT(value == 0);
        cleanup.cleanup();
        CUTE_ASSERT(value == 42);
        value = 0;
    }
    
    // check that the cleanup action was not triggered a second time
    CUTE_ASSERT(value == 0);
}
