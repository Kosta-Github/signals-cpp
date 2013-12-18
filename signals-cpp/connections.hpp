#pragma once

#include "signal.hpp"

namespace signals {

    /// The `connections` class helps in tracking and disconnecting a group of connections.
    /// On object destruction it disconnects all tracked connections automatically. In order
    /// to track a connection the `connect` method can be used with the actual `signal` as
    /// the first argument; all following arguments will be passed on to the `signal::connect`
    /// method. Or an existing `connection` object can be explicitly added to the list of
    /// tracked connections via the `add` method.
    ///
    /// E.g., a `connections` object can be used as a member variable in a class that
    /// connects itself to other signals. The `connections` member should therefore be best
    /// placed at the end of the class members of that class, so it gets destructed before
    /// all other members which ensures that all other data members are still available and
    /// valid to finish potential callback running in parallel on other threads. The 
    struct connections {
        inline connections()  { }
        inline ~connections() { disconnect_all(true); }

#if defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        /// Connects to the given `signal` `s` and adds the created `connection` to
        /// the list of tracked connections.
        template<typename SIGNAL, typename... ARGS>
        inline connection connect(SIGNAL& s, ARGS...&& args) {
            auto conn = s.connect(std::forward<ARGS...>(args...));
            add(conn);
            return conn;
        }

#else //  defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        template<typename SIGNAL, typename ARG1>
        inline connection connect(SIGNAL& s, ARG1&& arg1) {
            auto conn = s.connect(std::forward<ARG1>(arg1));
            add(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2>
        inline connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2));
            add(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2, typename ARG3>
        inline connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3));
            add(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
        inline connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3, ARG4&& arg4) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3), std::forward<ARG4>(arg4));
            add(conn);
            return conn;
        }

#endif //  defined(SIGNALS_CPP_HAVE_VARIADIC_TEMPLATES)

        /// If the given `connection` `conn` is connected it gets added to the list of
        /// tracked connections.
        inline void add(connection conn) {
            if(conn.connected()) {
                m_conns.emplace_back(std::move(conn));
            }
        }

        /// Disconnects all tracked `connections`.
        inline void disconnect_all(bool wait = false) {
            for(auto&& i : m_conns) { i.disconnect(false); } // first disconnect all connections without waiting
            if(wait) { for(auto&& i : m_conns) { i.disconnect(true); } } // then wait for them (if requested)
            m_conns.clear();
        }

#if defined(SIGNALS_CPP_NEED_EXPLICIT_MOVE)
    public:
        inline connections(connections&& o) : m_conns(std::move(o.m_conns)) { }
        inline connections& operator=(connections&& o) { m_conns = std::move(o.m_conns); return *this; }
#endif // defined(SIGNALS_CPP_NEED_EXPLICIT_MOVE)

    private:
        connections(connections const& o); // = delete;
        connections& operator=(connections const& o); // = delete;

    private:
        std::vector<connection> m_conns;
    };

} // namespace signals
