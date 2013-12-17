#include <catch/catch.hpp>

#include <signals-cpp/connections.hpp>
#include <signals-cpp/signal.hpp>

CATCH_TEST_CASE(
    "test a single simple connection",
    "[signals][connection][connect]"
) {
    signals::signal<void(int v)> sig;

    int value = 0;
    sig.connect([&](int v) { value = v; });

    sig.emit(42);
    CATCH_CHECK(value == 42);
}

CATCH_TEST_CASE(
    "test to disconnect a single simple connection",
    "[signals][connection][disconnection]"
) {
    signals::signal<void(int v)> sig;

    int value = 0;
    auto conn = sig.connect([&](int v) { value = v; });

    sig.emit(42);
    CATCH_CHECK(value == 42);

    CATCH_CHECK(sig.disconnect(conn));
    CATCH_CHECK_FALSE(sig.disconnect(conn)); // cannot be removed a second time
    sig.emit(84);
    CATCH_CHECK(value == 42); // still needs to be 42, since we disconnected
}

CATCH_TEST_CASE(
    "test to check the existance of a connection",
    "[signals][connection][exists]"
) {
    signals::signal<void(int v)> sig;

    auto connTrue = sig.connect([&](int) { });
    auto connFalse = signals::connection();

    CATCH_CHECK(sig.connected(connTrue));
    CATCH_CHECK_FALSE(sig.connected(connFalse));

    CATCH_CHECK(sig.disconnect(connTrue));
    CATCH_CHECK_FALSE(sig.connected(connTrue));
}

CATCH_TEST_CASE(
    "test multiple simple connections",
    "[signals][connection]"
) {
    signals::signal<void(int v)> sig;

    int value1 = 0, value2 = 0;
    sig.connect([&](int v) { value1 = v;     });
    sig.connect([&](int v) { value2 = v * 2; });

    sig.emit(42);
    CATCH_CHECK(value1 == 42);
    CATCH_CHECK(value2 == 84);
}

CATCH_TEST_CASE(
    "test the connections mediator",
    "[signals][connections]"
) {
    signals::signal<void(int v)> sig;
    signals::connections conns;

    int value1 = 0, value2 = 0;
    conns.connect(sig, [&](int v) { value1 = v;     });
    conns.connect(sig, [&](int v) { value2 = v * 2; });

    sig.emit(42);
    CATCH_CHECK(value1 == 42);
    CATCH_CHECK(value2 == 84);

    conns.disconnect_all();
    sig.emit(21);
    CATCH_CHECK(value1 == 42); // should be the same as above
    CATCH_CHECK(value2 == 84); // should be the same as above
}

CATCH_TEST_CASE(
    "test to connect to a method",
    "[signals][connection][method]"
) {
    struct Test {
        Test() : v(0) {
            conns.connect(sigVoid, this, &Test::onVoidValue);
            conns.connect(sigInt,  this, &Test::onIntValue);
        }

        void onVoidValue() { v = 1510; }
        void onIntValue(int v_) { v = v_; }

        int v;
        signals::signal<void()>    sigVoid;
        signals::signal<void(int)> sigInt;
        signals::connections conns;
    };

    Test t;
    CATCH_CHECK(t.v == 0);

    t.sigVoid.emit();
    CATCH_CHECK(t.v == 1510);

    t.sigInt.emit(42);
    CATCH_CHECK(t.v == 42);
}

CATCH_TEST_CASE(
    "test to connect to a method with derivation from signals::connections",
    "[signals][connections][method]"
) {
    struct Test : signals::connections {
        Test() : v(0) {
            connect(sigVoid, this, &Test::onVoidValue);
            connect(sigInt,  this, &Test::onIntValue);
        }

        void onVoidValue() { v = 1510; }
        void onIntValue(int v_) { v = v_; }

        int v;
        signals::signal<void()>    sigVoid;
        signals::signal<void(int)> sigInt;
    };

    Test t;
    CATCH_CHECK(t.v == 0);

    t.sigVoid.emit();
    CATCH_CHECK(t.v == 1510);

    t.sigInt.emit(42);
    CATCH_CHECK(t.v == 42);
}
