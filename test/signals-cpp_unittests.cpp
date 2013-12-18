#include <catch/catch.hpp>

#include <signals-cpp/connections.hpp>
#include <signals-cpp/signal.hpp>

#include "milestone.hpp"

CATCH_TEST_CASE(
    "test a single simple connection",
    "[signals][signals_01][single-threaded]"
) {
    signals::signal<void(int v)> sig;

    int value = 0;
    sig.connect([&](int v) { value = v; });

    sig.emit(42);
    CATCH_CHECK(value == 42);
}

CATCH_TEST_CASE(
    "test to disconnect a single simple connection",
    "[signals][signals_02][single-threaded]"
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
    "[signals][signals_03][single-threaded]"
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
    "[signals][signals_04][single-threaded]"
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
    "[signals][signals_05][single-threaded]"
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
    "[signals][signals_06][single-threaded]"
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
    "test to connect to a method with inheriting from signals::connections",
    "[signals][signals_07][single-threaded]"
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

CATCH_TEST_CASE(
    "test to disconnect a connection from within the callback triggered by that connection",
    "[signals][signals_08][single-threaded]"
) {
    signals::signal<void()> sig;
    signals::connection conn;

    conn = sig.connect([&]() { conn.disconnect(false); });
    CATCH_CHECK(conn.connected());
    sig.emit();
    CATCH_CHECK_FALSE(conn.connected());
}

CATCH_TEST_CASE(
    "test to disconnect a connection from within the callback triggered by that connection on another thread",
    "[signals][signals_09][multi-threaded]"
) {
    signals::signal<void()> sig;
    signals::connection conn;

    signals::test::milestone step;

    conn = sig.connect([&]() {
        step.execute_at(2, [&]() { conn.disconnect(false); });
        step.delay(4, std::chrono::milliseconds(10));
    });
    auto t = std::thread([&]() {
        step.reached(1);
        sig.emit();
        step.reached(5);
    });

    step.execute_at(0, [&]() { CATCH_CHECK(conn.connected()); });
    step.execute_at(3, [&]() { CATCH_CHECK_FALSE(conn.connected()); });
    CATCH_CHECK(step.returns_after(4, [&]() { conn.disconnect(true); }));
    step.reached(6);

    t.join();
}

CATCH_TEST_CASE(
    "test to connect a new connection from within the callback triggered by a signal",
    "[signals][signals_10][single-threaded]"
) {
    signals::signal<void()> sig;
    signals::connection conn1, conn2;
    int value = 0;

    conn1 = sig.connect([&]() {
        conn1.disconnect();
        conn2 = sig.connect([&]() { value = 42; });
    });

    CATCH_CHECK(conn1.connected());
    CATCH_CHECK_FALSE(conn2.connected());
    CATCH_CHECK(value == 0);

    sig.emit();
    CATCH_CHECK_FALSE(conn1.connected());
    CATCH_CHECK(conn2.connected());
    CATCH_CHECK(value == 0);

    sig.emit();
    CATCH_CHECK_FALSE(conn1.connected());
    CATCH_CHECK(conn2.connected());
    CATCH_CHECK(value ==42);
}

CATCH_TEST_CASE(
    "test to connect a new connection from within the callback triggered by a signal and immediately trigger "
    "this new connection from another thread without the need to finish the first trigger before that",
    "[signals][signals_11][multi-threaded]"
) {
    signals::signal<void()> sig;
    signals::connection conn1, conn2;
    int value = 0;

    signals::test::milestone step;

    conn1 = sig.connect([&]() {
        step.execute_at(2, [&]() { conn1.disconnect(); });
        step.execute_at(4, [&]() { conn2 = sig.connect([&]() { value = 42; }); });
        step.reached(9);
    });
    auto t1 = std::thread([&]() {
        step.reached(1);
        sig.emit();
        step.reached(10);
    });

    auto t2 = std::thread([&]() {
        step.reached(6);
        sig.emit();
        step.reached(7);
    });

    step.execute_at(0, [&]() {
        CATCH_CHECK(conn1.connected());
        CATCH_CHECK_FALSE(conn2.connected());
        CATCH_CHECK(value == 0);
    });

    step.execute_at(3, [&]() {
        CATCH_CHECK_FALSE(conn1.connected());
        CATCH_CHECK_FALSE(conn2.connected());
        CATCH_CHECK(value == 0);
    });

    step.execute_at(5, [&]() {
        CATCH_CHECK_FALSE(conn1.connected());
        CATCH_CHECK(conn2.connected());
        CATCH_CHECK(value == 0);
    });

    step.execute_at(8, [&]() {
        CATCH_CHECK_FALSE(conn1.connected());
        CATCH_CHECK(conn2.connected());
        CATCH_CHECK(value == 42);
    });

    step.reached(11);

    t1.join();
    t2.join();
}
