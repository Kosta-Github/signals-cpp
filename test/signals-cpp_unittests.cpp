#include <catch/catch.hpp>

#include <signals-cpp/signal.hpp>


struct StructWithSignal {
    StructWithSignal()  { std::cout << "StructWithSignal: ctor()" << std::endl; }
    ~StructWithSignal() { std::cout << "StructWithSignal: dtor()" << std::endl; }

public:
    signals::signal<void(int value)> signalIntValue;
};

CATCH_TEST_CASE(
    "test a single simple connection",
    "[signals][connection][connect]"
) {
    StructWithSignal a;

    int value = 0;
    a.signalIntValue.connect([&](int v) { value = v; });

    a.signalIntValue.emit(42);
    CATCH_CHECK(value == 42);
}

CATCH_TEST_CASE(
    "test to disconnect a single simple connection",
    "[signals][connection][disconnection]"
) {
    StructWithSignal a;

    int value = 0;
    auto conn = a.signalIntValue.connect([&](int v) { value = v; });

    a.signalIntValue.emit(42);
    CATCH_CHECK(value == 42);

    CATCH_CHECK(a.signalIntValue.disconnect(conn));
    CATCH_CHECK_FALSE(a.signalIntValue.disconnect(conn)); // cannot be removed a second time
    a.signalIntValue.emit(84);
    CATCH_CHECK(value == 42); // still needs to be 42, since we disconnected
}

CATCH_TEST_CASE(
    "test to check the existance of a connection",
    "[signals][connection][exists]"
) {
    StructWithSignal a;

    auto connTrue = a.signalIntValue.connect([&](int) { });
    auto connFalse = signals::connection();

    CATCH_CHECK(a.signalIntValue.connected(connTrue));
    CATCH_CHECK_FALSE(a.signalIntValue.connected(connFalse));

    CATCH_CHECK(a.signalIntValue.disconnect(connTrue));
    CATCH_CHECK_FALSE(a.signalIntValue.connected(connTrue));
}

CATCH_TEST_CASE(
    "test multiple simple connections",
    "[signals][connection]"
) {
    StructWithSignal a;

    int value1 = 0, value2 = 0;
    a.signalIntValue.connect([&](int v) { value1 = v;     });
    a.signalIntValue.connect([&](int v) { value2 = v * 2; });

    a.signalIntValue.emit(42);
    CATCH_CHECK(value1 == 42);
    CATCH_CHECK(value2 == 84);
}
