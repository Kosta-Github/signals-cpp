// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include "utils_file.hpp"

#include <chrono>
#include <cstdint>
#include <set>

namespace cute {
    namespace detail {

        inline auto trim(std::string const& s) -> std::string {
            auto start = 0;
            while(s[start] == ' ') { ++start; }
            if(start >= s.size()) { return ""; }
            auto end = s.size() - 1;
            while(s[end] == ' ') { --end; }
            return s.substr(start, end - start + 1);
        }

        inline auto parse_tags(std::string const& tags) -> std::set<std::string> {
            std::set<std::string> res;

            auto start = std::string::size_type(0);
            while(start < tags.size()) {
                auto end = tags.find(',', start);
                auto tag = trim(tags.substr(start, end - start));
                if(!tag.empty()) { res.insert(std::move(tag)); }
                start = ((end < tags.npos) ? end + 1 : tags.npos);
            }
            
            return res;
        }

        inline bool skip_test(
            std::set<std::string> const& test_tags,
            std::set<std::string> const& include_tags,
            std::set<std::string> const& exclude_tags
        ) {
            if(test_tags.empty()) { return false; }

            for(auto&& include : include_tags) {
                if(test_tags.find(include) == test_tags.end()) { return true; }
            }

            for(auto&& exclude : exclude_tags) {
                if(test_tags.find(exclude) != test_tags.end()) { return true; }
            }
            
            return false;
        }

        inline std::string xml_encode(
            std::string const& str
        ) {
            std::string res;
            res.reserve(2 * str.size());
            for(auto c : str) {
                switch(c) {
                    case '&':  res += "&amp;";  break;
                    case '\"': res += "&quot;"; break;
                    case '\'': res += "&apos;"; break;
                    case '<':  res += "&lt;";   break;
                    case '>':  res += "&gt;";   break;
                    default:   res += c;        break;
                }
            }
            return res;
        }
        
        inline auto time_now() -> decltype(std::chrono::high_resolution_clock::now()) {
            return std::chrono::high_resolution_clock::now();
        }

        template<typename T>
        inline std::size_t time_diff_ms(T&& start, T&& end) {
            auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            return static_cast<std::size_t>(diff);
        }

    } // namespace detail
} // namespace cute
