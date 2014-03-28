// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#define CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE)  NAME##LINE
#define CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, LINE)    CUTE_DETAIL_UNIQUE_NAME_LINE_2(NAME, LINE)
#define CUTE_DETAIL_UNIQUE_NAME(NAME)               CUTE_DETAIL_UNIQUE_NAME_LINE(NAME, __LINE__)

#define CUTE_DETAIL_ASSERT(EXPR_EVAL, FILE, LINE, EXPR_TEXT, CAPTURES1, CAPTURES2)                                                                      \
    try {                                                                                                                                               \
        ++cute::detail::eval_context::current().checks_performed;                                                                                       \
        if(!static_cast<bool>(EXPR_EVAL)) {                                                                                                             \
            cute::detail::eval_context::current().register_exception(                                                                                   \
                cute::exception(FILE, LINE, "validation failed: " EXPR_TEXT, "", CAPTURES1, CAPTURES2)                                                  \
            );                                                                                                                                          \
        }                                                                                                                                               \
    } catch(cute::exception const& ex) {                                                                                                                \
        cute::detail::eval_context::current().register_exception(ex);                                                                                   \
    } catch(std::exception const &ex) {                                                                                                                 \
        cute::detail::eval_context::current().register_exception(                                                                                       \
            cute::exception(FILE, LINE, "got an unexpected exception with message \"" + std::string(ex.what()) + "\"", EXPR_TEXT, CAPTURES1, CAPTURES2) \
        );                                                                                                                                              \
    } catch(...) {                                                                                                                                      \
        cute::detail::eval_context::current().register_exception(                                                                                       \
            cute::exception(FILE, LINE, "got an unexpected exception of unknown type", EXPR_TEXT, CAPTURES1, CAPTURES2)                                 \
        );                                                                                                                                              \
    }

#define CUTE_DETAIL_ASSERT_THROWS_AS(EXPR, EXCEPT, CAPTURES)                                                                        \
    {                                                                                                                               \
        auto CUTE_DETAIL_UNIQUE_NAME(exception_ok) = false;                                                                         \
        try {                                                                                                                       \
            ++cute::detail::eval_context::current().checks_performed;                                                               \
            static_cast<void>(EXPR);                                                                                                \
        } catch(EXCEPT const&) {                                                                                                    \
            CUTE_DETAIL_UNIQUE_NAME(exception_ok) = true;                                                                           \
        } catch(...) {                                                                                                              \
        }                                                                                                                           \
        if(!CUTE_DETAIL_UNIQUE_NAME(exception_ok)) {                                                                                \
            cute::detail::eval_context::current().register_exception(                                                               \
                cute::exception(__FILE__, __LINE__, "didn't get an expected exception of type \"" #EXCEPT "\"", #EXPR, CAPTURES)    \
            );                                                                                                                      \
        }                                                                                                                           \
    }

#define CUTE_DETAIL_INIT()                                                                                          \
    cute::detail::test_registry& cute::detail::test_registry::instance() { static test_registry reg; return reg; }  \
    cute::detail::eval_context* cute::detail::eval_context::g_current = nullptr;


#define CUTE_DETAIL_TEST(...)                                                           \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)();                            \
    static bool CUTE_DETAIL_UNIQUE_NAME(cute_detail_test_reg) =                         \
        cute::detail::test_registry::instance().add(cute::test(                         \
            __FILE__, __LINE__, CUTE_DETAIL_UNIQUE_NAME(cute_detail_test), __VA_ARGS__  \
        ));                                                                             \
    static void CUTE_DETAIL_UNIQUE_NAME(cute_detail_test)()

#define CUTE_DETAIL_DECOMPOSE(EXPR) static_cast<std::string>(cute::detail::decomposer()->* EXPR)
