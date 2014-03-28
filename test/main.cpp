//
// The MIT License (MIT)
//
// Copyright (c) 2013 by Konstantin (Kosta) Baumann & Autodesk Inc.
//
// Permission is hereby granted, free of charge,  to any person obtaining a copy of
// this software and  associated documentation  files  (the "Software"), to deal in
// the  Software  without  restriction,  including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software,  and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this  permission notice  shall be included in all
// copies or substantial portions of the Software.
//
// THE  SOFTWARE  IS  PROVIDED  "AS IS",  WITHOUT  WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE  AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE  LIABLE FOR ANY CLAIM,  DAMAGES OR OTHER LIABILITY, WHETHER
// IN  AN  ACTION  OF  CONTRACT,  TORT  OR  OTHERWISE,  ARISING  FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <cute/cute.hpp>
#include <cute/reporters/reporter_ide.hpp>

CUTE_INIT(); // initialize the cute framework

int main(int argc, char* argv[]) {
    // create the test suite execution context
    auto context = cute::context();

    // print out the status of each test during the execution of the test suite
    context.reporters.emplace_back(cute::reporter_ide);

    // run the test suite
    auto results = context.run();

    // print out the test suite result summary
    cute::reporter_ide_summary(results);

    // exit with an exit code indicating success or failure for the test suite
    return ((results.test_cases_failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS);
}
