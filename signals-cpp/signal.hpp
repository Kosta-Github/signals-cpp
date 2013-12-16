#pragma once

#include "connection.hpp"

#include <cassert>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

namespace signals {

    template<typename SIGNATURE>
    struct signal {

        ~signal() { disconnect_all(); }

        connection connect(std::function<SIGNATURE> target) {
            assert(target);

            std::lock_guard<std::mutex> lock(m_targets_mutex);

            m_targets.emplace_back(connection(std::make_shared<bool>(true)), std::move(target));
            return m_targets.back().conn;
        }

        bool disconnect(connection conn) {
            std::lock_guard<std::mutex> lock(m_targets_mutex);

            auto found = find(conn, m_targets);
            if(found == m_targets.end()) { return false; }

            conn.disconnect();
            m_targets.erase(found);

            return true;
        }

        void disconnect_all() {
            std::lock_guard<std::mutex> lock(m_targets_mutex);

            for(auto&& i : m_targets) { i.conn.disconnect(); }
            m_targets.clear();
        }

        bool connected(connection conn) const {
            std::lock_guard<std::mutex> lock(m_targets_mutex);

            return (find(conn, m_targets) != m_targets.end());
        }

        void emit() const {
            for(auto& i : copy_targets()) { if(i.conn.connected()) { i.target(); } }
        }

        template<typename ARG1>
        void emit(ARG1 const& arg1) const {
            for(auto& i : copy_targets()) { if(i.conn.connected()) { i.target(arg1); } }
        }

        template<typename ARG1, typename ARG2>
        void emit(ARG1 const& arg1, ARG1 const& arg2) const {
            for(auto& i : copy_targets()) { if(i.conn.connected()) { i.target(arg1, arg2); } }
        }

        template<typename ARG1, typename ARG2, typename ARG3>
        void emit(ARG1 const& arg1, ARG1 const& arg2, ARG1 const& arg3) const {
            for(auto& i : copy_targets()) { if(i.conn.connected()) { i.target(arg1, arg2, arg3); } }
        }

    private:
        struct connection_target {
            connection_target(connection c, std::function<SIGNATURE> t) : conn(std::move(c)), target(std::move(t)) { }

            connection conn;
            std::function<SIGNATURE> target;
        };

        mutable std::mutex m_targets_mutex;
        std::vector<connection_target> m_targets;

        // creates a copy of the currently registered connections under a lock of
        // the mutex; this is used to avoid potential dead-lock situations in case
        // of connecting a new target or disconnecting an existing connection during
        // the execution of the emit() action.
        auto copy_targets() const -> decltype(m_targets) {
            std::lock_guard<std::mutex> lock(m_targets_mutex);

            return m_targets;
        }

        template<typename CONT>
        static auto find(connection conn, CONT&& cont) -> decltype(cont.begin()) {
            // assumes that the mutex has been locked already!
            return std::find_if(
                cont.begin(), cont.end(),
                [&](connection_target const& i) { return (i.conn == conn); }
            );
        }

    };

} // namespace signals
