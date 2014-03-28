Multi-threaded tests
====================

When writing multi-threaded test cases the following pitfalls needs to be avoided:

1. Uncaught assertions (thrown by `cute` or from your code) in separate threads are leading to call `std::terminate()`: it must be ensured that all exceptions are caught on the separate thread. `cute::thread` can be used for this purpose. It is just a simple wrapper around `std::thread` and wraps the execution with a `catch-all-exceptions` handler signalling an unhandled exception as a test failure.

2. `std::thread`s need to be joined before their lifetime ends otherwise it leads to calling `std::terminate()`: `cute::thread` can be used for this purpose as well, since it ensures proper joining of the wrapped `std::thread` prior to it's destruction.

3. Test validation calls are not thread safe: `cute` has been designed right from it's beginning with multi-threading in mind and all test validation calls can be safely used in arbitrary threads.

```C++
CUTE_TEST("A passed check in a separate thread is detected") {
    auto t = cute::thread([&]() { CUTE_ASSERT(true); });
}

// this test will fail (as expected)
CUTE_TEST("A failed check in a separate thread is detected") {
    auto t = cute::thread([&]() { CUTE_ASSERT(false); });
}

// this test will fail (as expected)
CUTE_TEST("An unhandled exception in a cute::thread is detected") {
    auto t = cute::thread([&]() { throw std::runtime_error("forced exception"); });
}
```


TODO
====

- describe usage of `cute::tick`
