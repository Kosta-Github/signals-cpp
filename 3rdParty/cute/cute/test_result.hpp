// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "capture.hpp"

#include <memory>

namespace cute {

    enum class result_type {
        pass,
        skip,
        fail,
        fatal
    };

    struct test_result {
        cute::test const test;

        result_type result;
        std::size_t duration_ms;
        std::shared_ptr<exception const> excp;

        inline test_result(
            cute::test test_,
            result_type result_ = result_type::pass,
            std::size_t duration_ms_ = 0
        ) :
            test(std::move(test_)),
            result(std::move(result_)),
            duration_ms(std::move(duration_ms_))
        { }
    };

} // namespace cute
