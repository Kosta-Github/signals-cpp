// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "detail/utils_file.hpp"

#include <atomic>
#include <cassert>
#include <mutex>

namespace cute {

    struct test_suite_result {
        std::vector<test_result> const test_results;
        std::size_t const test_cases_passed;
        std::size_t const test_cases_skipped;
        std::size_t const test_cases_failed;
        std::size_t const checks_performed;
        std::size_t const duration_ms; // milliseconds

        inline test_suite_result(
            std::vector<test_result> test_results_,
            std::size_t checks_performed_,
            std::size_t duration_ms_
        ) :
            test_results(std::move(test_results_)),
            test_cases_passed(0), test_cases_skipped(0), test_cases_failed(0),
            checks_performed(checks_performed_),
            duration_ms(duration_ms_)
        {
            auto& passed    = const_cast<std::size_t&>(test_cases_passed);
            auto& skipped   = const_cast<std::size_t&>(test_cases_skipped);
            auto& failed    = const_cast<std::size_t&>(test_cases_failed);
            for(auto&& r : test_results) {
                switch(r.result) {
                    case result_type::pass:     ++passed;   break;
                    case result_type::skip:     ++skipped;  break;
                    case result_type::fail:     ++failed;   break;
                    case result_type::fatal:    ++failed;   break;
                    default:                    assert(false);
                }
            }
        }
    };

} // namespace cute
