#pragma once

#include "signal.hpp"

 // disable warning: class 'ABC' needs to have dll-interface to be used by clients of struct 'XYZ'
#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable: 4251)
#endif // defined(_MSC_VER)

namespace signals {

    struct connections {
        connections()  { }
        ~connections() { disconnect_all(); }

#if defined(SIGNALS_CPP_HAS_VARIADIC_TEMPLATES)

        template<typename SIGNAL, typename... ARGS>
        connection connect(SIGNAL& s, ARGS...&& args) {
            auto conn = s.connect(std::forward<ARGS...>(args...));
            m_conns.push_back(conn);
            return conn;
        }

#else //  defined(SIGNALS_CPP_HAS_VARIADIC_TEMPLATES)

        template<typename SIGNAL, typename ARG1>
        connection connect(SIGNAL& s, ARG1&& arg1) {
            auto conn = s.connect(std::forward<ARG1>(arg1));
            m_conns.push_back(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2>
        connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2));
            m_conns.push_back(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2, typename ARG3>
        connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3));
            m_conns.push_back(conn);
            return conn;
        }

        template<typename SIGNAL, typename ARG1, typename ARG2, typename ARG3, typename ARG4>
        connection connect(SIGNAL& s, ARG1&& arg1, ARG2&& arg2, ARG3&& arg3, ARG4&& arg4) {
            auto conn = s.connect(std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3), std::forward<ARG4>(arg4));
            m_conns.push_back(conn);
            return conn;
        }

#endif //  defined(SIGNALS_CPP_HAS_VARIADIC_TEMPLATES)

        void disconnect_all() {
            for(auto&& i : m_conns) { i.disconnect(); }
            m_conns.clear();
        }

#if defined(SIGNALS_CPP_NEED_EXPLICIT_MOVE)
    public:
        connections(connections&& o) : m_conns(std::move(o.m_conns)) { }
        connections& operator=(connections&& o) { m_conns = std::move(o.m_conns); return *this; }
#endif // defined(SIGNALS_CPP_NEED_EXPLICIT_MOVE)

    private:
        connections(connections const& o); // = delete;
        connections& operator=(connections const& o); // = delete;

    private:
        std::vector<connection> m_conns;
    };

} // namespace signals

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif // defined(_MSC_VER)
