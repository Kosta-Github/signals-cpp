// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "../to_string.hpp"

namespace cute {
    namespace detail {

        struct decomposer {

            template<typename T>
            struct expression {
                T const& obj;

                inline expression(T const& obj_) : obj(obj_) { }

                inline operator std::string() const { return to_string(obj); }

                template<typename R> inline std::string operator!() const { std::ostringstream os; os << '!' << to_string(obj); return os.str(); }

                template<typename R> inline std::string operator==(R const& rhs) const { return bin_op_to_string("==", rhs); }
                template<typename R> inline std::string operator!=(R const& rhs) const { return bin_op_to_string("!=", rhs); }
                template<typename R> inline std::string operator< (R const& rhs) const { return bin_op_to_string("<" , rhs); }
                template<typename R> inline std::string operator<=(R const& rhs) const { return bin_op_to_string("<=", rhs); }
                template<typename R> inline std::string operator> (R const& rhs) const { return bin_op_to_string(">" , rhs); }
                template<typename R> inline std::string operator>=(R const& rhs) const { return bin_op_to_string(">=", rhs); }

                template<typename R> inline std::string operator&&(R const& rhs) const { return bin_op_to_string("&&", rhs); }
                template<typename R> inline std::string operator||(R const& rhs) const { return bin_op_to_string("||", rhs); }

                template<typename R>
                inline std::string bin_op_to_string(char const* op, R const& rhs) const {
                    std::ostringstream os; os << to_string(obj) << ' ' << op << ' ' << to_string(rhs); return os.str();
                }
            };

            template<typename T>
            inline expression<T const&> operator->*(T const& op) const { return expression<T const&>(op); }
        };

    } // namespace detail
} // namespace cute
