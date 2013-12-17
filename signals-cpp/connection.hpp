#pragma once

#include "signals-cpp.hpp"

#include <atomic>
#include <memory>

 // disable warning: class 'ABC' needs to have dll-interface to be used by clients of struct 'XYZ'
#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable: 4251)
#endif // defined(_MSC_VER)

namespace signals {

    struct SIGNALS_API connection {
        connection(std::shared_ptr<std::atomic<bool>> c = nullptr) : m_connected(std::move(c)) { }

        bool operator==(connection const& o) const { return (m_connected == o.m_connected); }
        bool operator!=(connection const& o) const { return (m_connected != o.m_connected); }

        bool connected() const { return (m_connected && *m_connected); }
        void disconnect() { if(m_connected) { *m_connected = false; } }

    private:
        std::shared_ptr<std::atomic<bool>> m_connected;
    };

} // namespace signals

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif // defined(_MSC_VER)
