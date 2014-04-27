Simple IDE reporter
===================

A test runner using the provided simple IDE reporter would like like this:
```C++
#include <cute/cute.hpp>
#include <cute/reporters/reporter_ide.hpp>

CUTE_INIT(); // initialize the cute framework

int main_with_ide_reporter(int argc, char* argv[]) {
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
```


Simple JUnit reporter
=====================

A test runner using the provided `JUnit` reporter would like like this:
```C++
#include <cute/cute.hpp>
#include <cute/reporters/reporter_junit.hpp>

CUTE_INIT(); // initialize the cute framework

int main_with_junit_reporter(int argc, char* argv[]) {
    // create the test suite execution context
    auto context = cute::context();

    // run the test suite
    auto results = context.run();

    // print out the test suite result summary in JUnit XML format
    cute::reporter_junit_summary(std::cout, results);

    // exit with an exit code indicating success or failure for the test suite
    return ((results.test_cases_failed > 0) ? EXIT_FAILURE : EXIT_SUCCESS);
}
```

Both reporters could also be combined, e.g., by writing the test results into a separate `JUnit` XML results file.
