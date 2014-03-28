// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "../test_result.hpp"

#include <iostream>

namespace cute {

    namespace detail {

        inline std::string ide_make_file_line_string(
            std::string file,
            int line
        ) {
#if defined(__GNUG__)
            file += ":" + std::to_string(line) + ": ";
#else // defined(__GNUG__)
            file += "(" + std::to_string(line) + "): ";
#endif // defined(__GNUG__)
            return file;
        }

    } // namespace detail

    inline void reporter_ide_to_stream(
        std::ostream& out,
        test_result const& res,
        std::size_t test_index_cur,
        std::size_t test_index_max
    ) {
        auto type = std::string();
        switch(res.result) {
            case result_type::pass:     type = "pass: ";  break;
            case result_type::skip:     type = "skip: ";  break;
            case result_type::fail:     type = "error: "; break;
            case result_type::fatal:    type = "fatal: "; break;
            default:                    assert(false);
        }

        auto test_header = detail::ide_make_file_line_string(res.test.file, res.test.line) + type;
        out << test_header << res.test.name;
        if(test_index_max > 1) { out << " [" << (test_index_cur + 1) << "/" << test_index_max << "]"; }
        out << std::endl;

        if(res.result != result_type::skip) {
            out << test_header << "    duration:   " << res.duration_ms << " ms" << std::endl;
        }

        if(auto ex = res.excp.get()) {
            auto ex_header = detail::ide_make_file_line_string(ex->file, ex->line) + type;

            out << ex_header << "    reason:     " << ex->message << std::endl;
            if(!ex->expression.empty()) { out << ex_header << "    expression: " << ex->expression << std::endl; }

            for(auto&& c : ex->captures.list) {
                out << ex_header << "    with:       " << c.name;
                if(c.name != c.value) { out << " => " << c.value; }
                out << std::endl;
            }
        }
    }

    inline void reporter_ide(
        test_result const& res,
        std::size_t test_index_cur,
        std::size_t test_index_max
    ) {
        reporter_ide_to_stream((res.result == result_type::pass) ? std::cout : std::cerr, res, test_index_cur, test_index_max);
    }

    inline void reporter_ide_summary_to_stream(
        std::ostream& out,
        test_suite_result const& results
    ) {
        out << std::endl;
        out << "test suite summary:" << std::endl;
        out << results.test_cases_passed    << " out of " << results.test_results.size() << " tests passed."    << std::endl;
        out << results.test_cases_skipped   << " out of " << results.test_results.size() << " tests skipped."   << std::endl;
        out << results.test_cases_failed    << " out of " << results.test_results.size() << " tests failed."    << std::endl;
        out << results.checks_performed     << " checks performed."                                             << std::endl;
        out << results.duration_ms          << " ms used for the whole test suite."                             << std::endl;
    }

    inline void reporter_ide_summary(
        test_suite_result const& results
    ) {
        reporter_ide_summary_to_stream((results.test_cases_failed > 0) ? std::cout : std::cerr, results);
    }

} // namespace cute
