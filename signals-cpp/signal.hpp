#pragma once

#include "connection.hpp"

#include <algorithm>
#include <cassert>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace signals {

    template<typename SIGNATURE>
    struct signal {

        inline signal()  { }
        inline ~signal() { disconnect_all(); }

        inline connection connect(std::function<SIGNATURE> target) {
            assert(target);

            // lock the mutex for writing
            std::lock_guard<std::mutex> lock(m_write_targets_mutex);
            auto new_targets = std::make_shared<std::vector<connection_target>>();

            // copy existing and still active targets
            if(auto t = m_targets) {
                std::copy_if(
                    t->begin(), t->end(),
                    std::back_inserter(*new_targets),
                    [](connection_target const& i) { return i.conn.connected(); }
                );
            }

            // add the new target to the new vector
            auto conn = connection::make_connection();
            new_targets->emplace_back(conn, std::move(target));

            // replace the pointer to the targets (in a thread safe manner)
            m_targets = new_targets;

            return conn;
        }

#if defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        template<typename OBJ, typename... ARGS>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARGS... args)) {
            assert(obj);
            assert(method);
            return connect([=](ARGS... args) { (obj->*method)(args...); });
        }

#else // defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        template<typename OBJ>
        inline connection connect(OBJ* obj, void (OBJ::*method)()) {
            assert(obj);
            assert(method);
            return connect([=]() { (obj->*method)(); });
        }

        template<typename OBJ, typename ARG1>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARG1 arg1)) {
            assert(obj);
            assert(method);
            return connect([=](ARG1 arg1) { (obj->*method)(arg1); });
        }

        template<typename OBJ, typename ARG1, typename ARG2>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARG1 arg1, ARG2 arg2)) {
            assert(obj);
            assert(method);
            return connect([=](ARG1 arg1, ARG2 arg2) { (obj->*method)(arg1, arg2); });
        }

        template<typename OBJ, typename ARG1, typename ARG2, typename ARG3>
        inline connection connect(OBJ* obj, void (OBJ::*method)(ARG1 arg1, ARG2 arg2, ARG3 arg3)) {
            assert(obj);
            assert(method);
            return connect([=](ARG1 arg1, ARG2 arg2, ARG3 arg3) { (obj->*method)(arg1, arg2, arg3); });
        }

#endif // defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        inline bool disconnect(connection conn) {
            if(!conn.connected()) { return false; }

            // lock the mutex for writing
            std::lock_guard<std::mutex> lock(m_write_targets_mutex);

            // try to find the matching target
            if(auto t = m_targets) {
                auto found = find(conn, *t);
                if(found != t->end()) {

                    // disconnect the connection but keep it in the targets list
                    conn.disconnect();

                    return true;
                }
            }

            // not found
            return false;
        }

        inline void disconnect_all() {
            // lock the mutex for writing
            std::lock_guard<std::mutex> lock(m_write_targets_mutex);

            // disconnect all targets
            if(auto t = m_targets) {
                // clean out the targets pointer so no other thread
                // will emit this signal anymore (running emit calls
                // might still reference the targets)
                m_targets = nullptr;

                // and mark all connected connections as being disconnected
                for(auto&& i : *t) { i.conn.disconnect(); }
            }
        }

        inline bool connected(connection conn) const {
            // if the connection is not connected to something it cannot
            // be connected this signal
            if(!conn.connected()) { return false; }

            // check if we can find the requested connection in the list of
            // stored connections
            if(auto t = m_targets) {
                return (find(conn, *t) != t->end());
            }

            return false;
        }

#if defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        template<typename... ARGS>
        inline void emit(ARGS... const& args) const {
            if(auto t = m_targets) {
                for(auto& i : *t) { if(i.conn.connected()) { i.target(args...); } }
            }
        }

#else // defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        inline void emit() const {
            if(auto t = m_targets) {
                for(auto& i : *t) { if(i.conn.connected()) { i.target(); } }
            }
        }

        template<typename ARG1>
        inline void emit(ARG1 const& arg1) const {
            if(auto t = m_targets) {
                for(auto& i : *t) { if(i.conn.connected()) { i.target(arg1); } }
            }
        }

        template<typename ARG1, typename ARG2>
        inline void emit(ARG1 const& arg1, ARG1 const& arg2) const {
            if(auto t = m_targets) {
                for(auto& i : *t) { if(i.conn.connected()) { i.target(arg1, arg2); } }
            }
        }

        template<typename ARG1, typename ARG2, typename ARG3>
        inline void emit(ARG1 const& arg1, ARG1 const& arg2, ARG1 const& arg3) const {
            if(auto t = m_targets) {
                for(auto& i : *t) { if(i.conn.connected()) { i.target(arg1, arg2, arg3); } }
            }
        }

#endif // defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

#if defined(SIGNALS_CPP_NEED_EXPLICIT_MOVE)
    public:
        inline signal(signal&& o) SIGNALS_CPP_NOEXCEPT :
            m_write_targets_mutex(std::move(o.m_write_targets_mutex)),
            m_targets(std::move(o.m_targets))
        { }

        inline signal& operator=(signal&& o) SIGNALS_CPP_NOEXCEPT {
            m_write_targets_mutex = std::move(o.m_write_targets_mutex);
            m_targets = std::move(o.m_targets);
            return *this;
        }
#endif // defined(SIGNALS_CPP_NEED_EXPLICIT_MOVE)

    private:
        signal(signal const& o); // = delete;
        signal& operator=(signal const& o); // = delete;

    private:
        struct connection_target {
            inline connection_target(connection c, std::function<SIGNATURE> t) :
                conn(std::move(c)), target(std::move(t))
            { }

            connection conn;
            std::function<SIGNATURE> target;
        };

        mutable std::mutex m_write_targets_mutex;
        std::shared_ptr<std::vector<connection_target>> m_targets;

        template<typename CONT>
        inline static auto find(connection conn, CONT&& cont) -> decltype(cont.begin()) {
            return std::find_if(
                cont.begin(), cont.end(),
                [&](connection_target const& i) { return (i.conn == conn); }
            );
        }

    };

} // namespace signals
