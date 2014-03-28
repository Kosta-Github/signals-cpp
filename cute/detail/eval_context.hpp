// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "../exception.hpp"
#include "../test.hpp"
#include "../test_result.hpp"
#include "../test_suite_result.hpp"
#include "utils_file.hpp"

#include <vector>

namespace cute {
    namespace detail {

        struct eval_context {
            eval_context() :
                reporters(nullptr), current_test(nullptr),
                checks_performed(0), duration_ms(0),
                m_previous(g_current)
            {
                g_current = this;
            }

            ~eval_context() {
                assert(g_current == this);
                g_current = m_previous;
            }

            std::vector<std::function<void(test_result const& rep, std::size_t test_index_cur, std::size_t test_index_max)>> const* reporters;

            test const* current_test;

            std::atomic<std::size_t> checks_performed;
            std::size_t duration_ms;

            std::mutex exceptions_mutex;
            std::shared_ptr<exception> excp;
            inline void register_exception(exception ex, bool throw_exception = true) {
                std::lock_guard<std::mutex> lock(exceptions_mutex);
                if(excp) {
                    if(throw_exception) { throw ex; }
                    return;
                }
                excp = std::make_shared<exception>(std::move(ex));
                if(throw_exception) {
                    throw *excp;
                }
            }

            std::mutex temp_folder_mutex;
            std::string temp_folder;
            inline std::string create_temp_folder() {
                std::lock_guard<std::mutex> lock(temp_folder_mutex);
                if(!temp_folder.empty()) { return temp_folder; }
                temp_folder = cute::detail::create_temp_folder();
                return temp_folder;
            }
            inline bool delete_temp_folder() {
                std::lock_guard<std::mutex> lock(temp_folder_mutex);
                if(temp_folder.empty()) { return true; }
                auto res = cute::detail::delete_folder(temp_folder);
                temp_folder.clear();
                return res;
            }

            std::vector<test_result> test_results;

            inline test_suite_result result() {
                return test_suite_result(
                    std::move(test_results), checks_performed, duration_ms
                );
            }

            static eval_context* g_current;
            eval_context* const m_previous;
            inline static eval_context& current() { assert(g_current); return *g_current; }
        };

    } // namespace detail
} // namespace cute
