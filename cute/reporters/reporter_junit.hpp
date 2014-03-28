// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "reporter_ide.hpp"
#include "../test_result.hpp"
#include "../test_suite_result.hpp"

#include <ostream>

namespace cute {
    namespace detail {

        inline void junit_write_lead_in(
            std::ostream& out,
            test_suite_result const& suite
        ) {
            out << "<testsuites>" << std::endl;
            out << "  <testsuite ";
            out <<    "errors=\"" << suite.test_cases_passed << "\" ";
            out <<    "tests=\"" << suite.test_results.size() << "\" ";
            out <<    "time=\"" << (suite.duration_ms / 1000.0f) << "\" ";
            out << ">" << std::endl;
        }

        inline void junit_write_test(
            std::ostream& out,
            test_result const& test
        ) {
            // extract bse filename part from full source filename
            auto name = test.test.file;
            auto pos1 = name.find_last_of("/\\");
            if(pos1 != name.npos) { name = name.substr(pos1 + 1); }
            auto pos2 = name.find_last_of(".");
            name = name.substr(0, pos2);

            out << "    <testcase ";
            out <<      "classname=\"" << xml_encode(name) << "\" ";
            out <<      "name=\"" << xml_encode(test.test.name) << "\" ";
            out <<      "time=\"" << (test.duration_ms / 1000.0f) << "\" ";
            out <<      ">" << std::endl;

            if(test.result == result_type::skip) {
                out << "      <skipped/>" << std::endl;
            }

            if(auto ex = test.excp.get()) {
                std::string type;
                switch(test.result) {
                    case result_type::pass:     type = "pass";  break;
                    case result_type::skip:     type = "skip";  break;
                    case result_type::fail:     type = "fail";  break;
                    case result_type::fatal:    type = "fatal"; break;
                    default:                    assert(false);
                }

                std::ostringstream text;
                reporter_ide_to_stream(text, test, 0, 1);

                out << "      <error ";
                out <<        "type=\"" << type << "\" ";
                out <<        "message=\"" << xml_encode(ex->message) << "\" ";
                out <<        ">" << std::endl;
                out << xml_encode(text.str());
                out << "      </error>" << std::endl;
            }

            out << "    </testcase>" << std::endl;
        }

        inline void junit_write_lead_out(
            std::ostream& out
        ) {
            out << "  </testsuite>" << std::endl;
            out << "/<testsuites>" << std::endl;
        }

    } // namespace detail

    inline std::ostream& reporter_junit_summary(
        std::ostream& out,
        test_suite_result const& suite
    ) {
        detail::junit_write_lead_in(out, suite);

        // write resut for each test
        for(auto&& test : suite.test_results) {
            detail::junit_write_test(out, test);
        }

        detail::junit_write_lead_out(out);

        return out;
    }

} // namespace cute
