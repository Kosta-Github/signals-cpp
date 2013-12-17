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

        template<typename SIGNAL, typename TARGET>
        connection connect(SIGNAL& s, TARGET&& target) {
            auto conn = s.connect(std::forward<TARGET>(target));
            m_conns.push_back(conn);
            return conn;
        }

        template<typename SIGNAL, typename OBJ, typename ARG1>
        connection connect(SIGNAL& s, OBJ&& obj, ARG1&& arg1) {
            auto conn = s.connect(std::forward<OBJ>(obj), std::forward<ARG1>(arg1));
            m_conns.push_back(conn);
            return conn;
        }

        template<typename SIGNAL, typename OBJ, typename ARG1, typename ARG2>
        connection connect(SIGNAL& s, OBJ&& obj, ARG1&& arg1, ARG2&& arg2) {
            auto conn = s.connect(std::forward<OBJ>(obj), std::forward<ARG1>(arg1), std::forward<ARG2>(arg2));
            m_conns.push_back(conn);
            return conn;
        }

        template<typename SIGNAL, typename OBJ, typename ARG1, typename ARG2, typename ARG3>
        connection connect(SIGNAL& s, OBJ&& obj, ARG1&& arg1, ARG1&& arg2, ARG1&& arg3) {
            auto conn = s.connect(std::forward<OBJ>(obj), std::forward<ARG1>(arg1), std::forward<ARG2>(arg2), std::forward<ARG3>(arg3));
            m_conns.push_back(conn);
            return conn;
        }

        void disconnect_all() {
            for(auto&& i : m_conns) { i.disconnect(); }
            m_conns.clear();
        }

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
