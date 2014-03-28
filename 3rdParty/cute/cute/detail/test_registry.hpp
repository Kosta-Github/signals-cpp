// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "../test.hpp"

#include <vector>

namespace cute {
    namespace detail {

        struct test_registry {
            std::vector<test> tests;
            inline bool add(test t) {
                tests.emplace_back(std::move(t)); return true;
            }

            static test_registry& instance();
        };

    } // namespace detail
} // namespace cute
