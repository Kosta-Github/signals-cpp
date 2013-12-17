#pragma once

#include "signals-cpp.hpp"

#include <atomic>
#include <memory>
#include <thread>

namespace signals {

    /// The `connection` class is just an abstract handle or representation for
    /// a connection between a signal and the corresponding target callback or slot.
    /// It can be checked if the connection is still connected and it could be
    /// disconnected.
    struct connection {
        struct data {
            inline data() : connected(true), running(0) { }

            std::atomic<bool> connected;    // connection still active?
            std::atomic<int>  running;      // number of currently active calls routed through this connection
        };

    public:
        inline connection() { }
        inline connection(std::shared_ptr<data> d) : m_data(std::move(d)) { }

        inline bool operator==(connection const& o) const { return (m_data == o.m_data); }
        inline bool operator!=(connection const& o) const { return (m_data != o.m_data); }

        /// Checks if the `connection` represented by this object is (still) connected.
        inline bool connected() const { return (m_data && m_data->connected); }

        /// Disconnects this `connection`. After the `disconnect` call the corresponding
        /// target callback will not be triggered anymore. If there are currently some
        /// active calls running via this `connection` the `disconnect` call blocks until
        /// all calls have finished.
        inline void disconnect() {
            if(!m_data) { return; }

            // first signal that no new call should be routed through this connection
            m_data->connected = false;

            // wait until all currently running calls have finished
            while(m_data->running > 0) { std::this_thread::yield(); } // busy loop
        }

    public:
        // only for internal use
        template<typename CB>
        inline void call(CB&& cb) {
            if(!m_data || !m_data->connected) { return; }

            ++m_data->running;
            if(m_data->connected) { cb(); }
            --m_data->running;
        }

        // only for internal use
        inline static connection make_connection() {
            return connection(std::make_shared<data>());
        }

    private:
        std::shared_ptr<data> m_data;
    };

} // namespace signals
