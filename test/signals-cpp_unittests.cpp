#include <catch/catch.hpp>

#include <signals-cpp/connections.hpp>
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

CATCH_TEST_CASE(
    "test the connections mediator",
    "[signals][connections]"
) {
    StructWithSignal a;
    signals::connections conns;

    int value1 = 0, value2 = 0;
    conns.connect(a.signalIntValue, [&](int v) { value1 = v;     });
    conns.connect(a.signalIntValue, [&](int v) { value2 = v * 2; });

    a.signalIntValue.emit(42);
    CATCH_CHECK(value1 == 42);
    CATCH_CHECK(value2 == 84);

    conns.disconnect_all();
    a.signalIntValue.emit(21);
    CATCH_CHECK(value1 == 42); // should be the same as above
    CATCH_CHECK(value2 == 84); // should be the same as above
}

CATCH_TEST_CASE(
    "test to connect to a method",
    "[signals][connection][method]"
) {
    struct Test {
        Test() : v(0) {
            conns.connect(sig, this, &Test::onIntValue);
        }

        void onIntValue(int v_) { v = v_; }

        int v;
        signals::signal<void(int)> sig;
        signals::connections conns;
    };

    Test t;

    t.sig.emit(42);

    CATCH_CHECK(t.v == 42);
}

CATCH_TEST_CASE(
    "test to connect to a method with derivation from signals::connections",
    "[signals][connections][method]"
) {
    struct Test : signals::connections {
        Test() : v(0) {
            connect(sig, this, &Test::onIntValue);
        }

        void onIntValue(int v_) { v = v_; }

        int v;
        signals::signal<void(int)> sig;
    };

    Test t;

    t.sig.emit(42);

    CATCH_CHECK(t.v == 42);
}
