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

#include <cute/cute.hpp>

#include <signals-cpp/connections.hpp>

CUTE_TEST(
    "test a single simple connection",
    "[signals],[signals_01],[single-threaded]"
) {
    signals::signal<void(int v)> sig;

    int value = 0;
    sig.connect([&](int v) { value = v; });

    sig.fire(42);
    CUTE_ASSERT(value == 42);
}

CUTE_TEST(
    "test to disconnect a single simple connection",
    "[signals],[signals_02],[single-threaded]"
) {
    signals::signal<void(int v)> sig;

    int value = 0;
    auto conn = sig.connect([&](int v) { value = v; });

    sig.fire(42);
    CUTE_ASSERT(value == 42);

    CUTE_ASSERT(conn.disconnect());
    CUTE_ASSERT(!conn.disconnect()); // cannot be disconnected a second time
    sig.fire(84);
    CUTE_ASSERT(value == 42); // still needs to be 42, since we disconnected
}

CUTE_TEST(
    "test to check the existance of a connection",
    "[signals],[signals_03],[single-threaded]"
) {
    signals::signal<void(int v)> sig;

    auto connTrue = sig.connect([&](int) { });
    auto connFalse = signals::connection();

    CUTE_ASSERT(connTrue.connected());
    CUTE_ASSERT(!connFalse.connected());

    CUTE_ASSERT(connTrue.disconnect());
    CUTE_ASSERT(!connTrue.connected());
}

CUTE_TEST(
    "test multiple simple connections",
    "[signals],[signals_04],[single-threaded]"
) {
    signals::signal<void(int v)> sig;

    int value1 = 0, value2 = 0;
    sig.connect([&](int v) { value1 = v;     });
    sig.connect([&](int v) { value2 = v * 2; });

    sig.fire(42);
    CUTE_ASSERT(value1 == 42);
    CUTE_ASSERT(value2 == 84);
}

CUTE_TEST(
    "test the connections mediator",
    "[signals],[signals_05],[single-threaded]"
) {
    signals::signal<void(int v)> sig;
    signals::connections conns;

    int value1 = 0, value2 = 0;
    conns.connect(sig, [&](int v) { value1 = v;     });
    conns.connect(sig, [&](int v) { value2 = v * 2; });

    sig.fire(42);
    CUTE_ASSERT(value1 == 42);
    CUTE_ASSERT(value2 == 84);

    conns.disconnect_all();
    sig.fire(21);
    CUTE_ASSERT(value1 == 42); // should be the same as above
    CUTE_ASSERT(value2 == 84); // should be the same as above
}

CUTE_TEST(
    "test to connect to a method",
    "[signals],[signals_06],[single-threaded]"
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
    CUTE_ASSERT(t.v == 0);

    t.sigVoid.fire();
    CUTE_ASSERT(t.v == 1510);

    t.sigInt.fire(42);
    CUTE_ASSERT(t.v == 42);
}

CUTE_TEST(
    "test to connect to a method with inheriting from signals::connections",
    "[signals],[signals_07],[single-threaded]"
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
    CUTE_ASSERT(t.v == 0);

    t.sigVoid.fire();
    CUTE_ASSERT(t.v == 1510);

    t.sigInt.fire(42);
    CUTE_ASSERT(t.v == 42);
}

CUTE_TEST(
    "test to disconnect a connection from within the callback triggered by that connection",
    "[signals],[signals_08],[single-threaded]"
) {
    signals::signal<void()> sig;
    signals::connection conn;

    conn = sig.connect([&]() { conn.disconnect(false); });
    CUTE_ASSERT(conn.connected());
    sig.fire();
    CUTE_ASSERT(!conn.connected());
}

CUTE_TEST(
    "test to disconnect a connection from within the callback triggered by that connection on another thread",
    "[signals],[signals_09],[multi-threaded]"
) {
    signals::signal<void()> sig;
    signals::connection conn;

    cute::tick ticker;

    conn = sig.connect([&]() {
        ticker.at_tick(2, [&]() { conn.disconnect(false); });
        ticker.delay_tick_for(4, std::chrono::milliseconds(10));
    });
    auto t = cute::thread([&]() {
        ticker.reached_tick(1);
        sig.fire();
        ticker.reached_tick(5);
    });

    ticker.at_tick(0, [&]() { CUTE_ASSERT(conn.connected()); });
    ticker.at_tick(3, [&]() { CUTE_ASSERT(!conn.connected()); });
    ticker.blocks_until_tick(4, [&]() { conn.disconnect(true); });
    ticker.reached_tick(6);
}

CUTE_TEST(
    "test to connect a new connection from within the callback triggered by a signal",
    "[signals],[signals_10],[single-threaded]"
) {
    signals::signal<void()> sig;
    signals::connection conn1, conn2;
    int value = 0;

    conn1 = sig.connect([&]() {
        conn1.disconnect();
        conn2 = sig.connect([&]() { value = 42; });
    });

    CUTE_ASSERT(conn1.connected());
    CUTE_ASSERT(!conn2.connected());
    CUTE_ASSERT(value == 0);

    sig.fire();
    CUTE_ASSERT(!conn1.connected());
    CUTE_ASSERT(conn2.connected());
    CUTE_ASSERT(value == 0);

    sig.fire();
    CUTE_ASSERT(!conn1.connected());
    CUTE_ASSERT(conn2.connected());
    CUTE_ASSERT(value ==42);
}

CUTE_TEST(
    "test to connect a new connection from within the callback triggered by a signal and immediately trigger "
    "this new connection from another thread without the need to finish the first trigger before that",
    "[signals],[signals_11],[multi-threaded]"
) {
    signals::signal<void()> sig;
    signals::connection conn1, conn2;
    int value = 0;

    cute::tick ticker;

    conn1 = sig.connect([&]() {
        ticker.at_tick(2, [&]() { conn1.disconnect(); });
        ticker.at_tick(4, [&]() { conn2 = sig.connect([&]() { value = 42; }); });
        ticker.reached_tick(9);
    });
    auto t1 = cute::thread([&]() {
        ticker.reached_tick(1);
        sig.fire();
        ticker.reached_tick(10);
    });

    auto t2 = cute::thread([&]() {
        ticker.reached_tick(6);
        sig.fire();
        ticker.reached_tick(7);
    });

    ticker.at_tick(0, [&]() {
        CUTE_ASSERT(conn1.connected());
        CUTE_ASSERT(!conn2.connected());
        CUTE_ASSERT(value == 0);
    });

    ticker.at_tick(3, [&]() {
        CUTE_ASSERT(!conn1.connected());
        CUTE_ASSERT(!conn2.connected());
        CUTE_ASSERT(value == 0);
    });

    ticker.at_tick(5, [&]() {
        CUTE_ASSERT(!conn1.connected());
        CUTE_ASSERT(conn2.connected());
        CUTE_ASSERT(value == 0);
    });

    ticker.at_tick(8, [&]() {
        CUTE_ASSERT(!conn1.connected());
        CUTE_ASSERT(conn2.connected());
        CUTE_ASSERT(value == 42);
    });

    ticker.reached_tick(11);
}
