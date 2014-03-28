// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#pragma once

#include <string>
#include <vector>

namespace cute {

    struct capture {
        std::string const name;
        std::string const value;

        inline capture(std::string name_ = "", std::string value_ = "") : name(std::move(name_)), value(std::move(value_)) { }
    };
    
    struct captures {
        inline captures() { }

        inline captures(
            capture cap0_,
            capture cap1_ = capture(),
            capture cap2_ = capture(),
            capture cap3_ = capture(),
            capture cap4_ = capture(),
            capture cap5_ = capture(),
            capture cap6_ = capture(),
            capture cap7_ = capture(),
            capture cap8_ = capture(),
            capture cap9_ = capture()
        ) {
            add(std::move(cap0_));
            add(std::move(cap1_));
            add(std::move(cap2_));
            add(std::move(cap3_));
            add(std::move(cap4_));
            add(std::move(cap5_));
            add(std::move(cap6_));
            add(std::move(cap7_));
            add(std::move(cap8_));
            add(std::move(cap9_));
        }

        inline captures(
            captures caps0_,
            captures caps1_
        ) {
            for(auto&& c : caps0_.list) { add(std::move(c)); }
            for(auto&& c : caps1_.list) { add(std::move(c)); }
        }

        inline void add(capture cap) {
            if(!cap.name.empty()) { list.emplace_back(std::move(cap)); }
        }
        
        std::vector<capture> list;
    };

} // namespace cute
