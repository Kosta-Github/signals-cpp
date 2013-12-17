#pragma once

#include "signals-cpp.hpp"

#include <atomic>
#include <memory>

namespace signals {

    /// The `connection` class is just an abstract handle or representation for
    /// a connection between a signal and the corresponding target callback or slot.
    /// It can be checked if the connection is still connected and it could be
    /// disconnected.
    struct connection {
        inline connection(std::shared_ptr<std::atomic<bool>> c = nullptr) : m_connected(std::move(c)) { }

        inline bool operator==(connection const& o) const { return (m_connected == o.m_connected); }
        inline bool operator!=(connection const& o) const { return (m_connected != o.m_connected); }

        /// Checks if the `connection` represented by this object is (still) connected.
        inline bool connected() const { return (m_connected && *m_connected); }

        /// Disconnects this `connection`. After the `disconnect` call the corresponding
        /// callback target will not be triggered anymore.
        inline void disconnect() { if(m_connected) { *m_connected = false; } }

    private:
        // The `shared_ptr` is used to define the connection identity
        // used by the operator== and for searching in stored connection lists.
        // The `atomic bool` is used for signaling that the connection should
        // be considered as disconnected (value equal to `false`) in a
        // thread-safe manner.
        std::shared_ptr<std::atomic<bool>> m_connected;
    };

} // namespace signals
