// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "capture.hpp"

#include <cassert>
#include <string>

namespace cute {

    // The cute exception is not derived from std::exception by intend,
    // otherwise we could not differentiate within CUTE_THROWS_AS()
    // between expected std::exceptions and cute::exceptions.
    struct exception {
        std::string const file;
        int const line;
        std::string const message;
        std::string const expression;
        cute::captures const captures;

        inline exception(
            std::string file_,
            int line_,
            std::string message_,
            std::string expression_ = "",
            cute::captures caps1_ = cute::captures(),
            cute::captures caps2_ = cute::captures()
        ) :
            file(std::move(file_)),
            line(std::move(line_)),
            message(std::move(message_)),
            expression(std::move(expression_)),
            captures(std::move(caps1_), std::move(caps2_))
        {
            assert(!message.empty());
        }
    };

} // namespace cute
