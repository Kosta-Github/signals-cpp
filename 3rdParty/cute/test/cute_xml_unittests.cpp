// Copyright 2014 by Konstantin (Kosta) Baumann
//
// Distributed under the MIT license (See accompanying file LICENSE.txt)
//

#include "../cute/cute.hpp"

CUTE_TEST("'xml_encode()' escapes all required chars", "pass") {
    CUTE_ASSERT(cute::detail::xml_encode("&") == "&amp;");
    CUTE_ASSERT(cute::detail::xml_encode("\"") == "&quot;");
    CUTE_ASSERT(cute::detail::xml_encode("\'") == "&apos;");
    CUTE_ASSERT(cute::detail::xml_encode("<") == "&lt;");
    CUTE_ASSERT(cute::detail::xml_encode(">") == "&gt;");
}

CUTE_TEST("'xml_encode()' escapes all required chars within a string", "pass") {
    CUTE_ASSERT(cute::detail::xml_encode("a&\"\'<>z") == "a&amp;&quot;&apos;&lt;&gt;z");
}
