// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "detail/macros_impl.hpp"

#define CUTE_INIT()         CUTE_DETAIL_INIT()
#define CUTE_TEST(...)      CUTE_DETAIL_TEST(__VA_ARGS__)
#define CUTE_CAPTURE(EXPR)  cute::capture(#EXPR, CUTE_DETAIL_DECOMPOSE(EXPR))

#define CUTE_ASSERT(EXPR, ...)                      CUTE_DETAIL_ASSERT(EXPR,                            __FILE__, __LINE__, #EXPR, CUTE_CAPTURE(EXPR),          cute::captures(__VA_ARGS__))
#define CUTE_ASSERT_THROWS_NOT(EXPR, ...)           CUTE_DETAIL_ASSERT((static_cast<void>(EXPR), true), __FILE__, __LINE__, #EXPR, cute::captures(__VA_ARGS__), cute::captures())
#define CUTE_ASSERT_THROWS(EXPR, ...)               CUTE_DETAIL_ASSERT_THROWS_AS(EXPR, std::exception, cute::captures(__VA_ARGS__))
#define CUTE_ASSERT_THROWS_AS(EXPR, EXCEPT, ...)    CUTE_DETAIL_ASSERT_THROWS_AS(EXPR, EXCEPT,         cute::captures(__VA_ARGS__))
