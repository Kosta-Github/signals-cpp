// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <memory>
#include <sstream>
#include <string>

namespace cute {

    inline std::string to_string(std::nullptr_t const&)     { return "nullptr"; }
    inline std::string to_string(std::string    const& str) { return ('\"' + str + '\"'); }
    inline std::string to_string(char const*    const& str) { return (str ? ('\"' + std::string(str) + '\"') : "nullptr"); }
    inline std::string to_string(char           const& c)   { return ('\'' + std::string(1, c) + '\''); }
    inline std::string to_string(bool           const& b)   { return (b ? "true" : "false"); }

    template<typename T>
    inline auto to_string(T const& value) -> std::string { std::ostringstream os; os << value; return os.str(); }

    template<typename T>
    inline std::string to_string(std::shared_ptr<T> const& p) { std::ostringstream os; os << p.get(); return os.str(); }

    template<typename T>
    inline std::string to_string(std::unique_ptr<T> const& p) { std::ostringstream os; os << p.get(); return os.str(); }

    template<typename T>
    inline std::string to_string(std::weak_ptr<T> const& p) { std::ostringstream os; os << p.get(); return os.str(); }

    inline std::string to_string(std::type_info const& ti) { return ti.name(); }

} // namespace cute
