cute: C++ unit test executor [![Build Status](https://travis-ci.org/Kosta-Github/cute.png)](https://travis-ci.org/Kosta-Github/cute)
============================

Feature set:
- uses C++11 features
- no external dependencies
- header-only
- supports multi-threaded test cases and is thread-safe
- supports tags for test case filtering
- can use custom reporters (provided: simple ide/console reporter, JUnit XML reporter)
- supports per test case temporary folder with automatic cleanup
- planned: provides capabilities for mocking classes and interfaces


status: beta
============

The development has just started and is not considered to be production-ready at the moment.


usage
=====

- [Writing tests](docs/writing_tests.md)
- [Test runners and reporters](docs/test_runners_and_reporters.md)
- [Using temporary folders](docs/temp_folders.md)
- [Multi-threaded tests](docs/multi_threaded_tests.md)


related work
============

`cute is based on the following ideas and frameworks:
- [lest](https://github.com/martinmoene/lest)
- [catch](https://github.com/philsquared/Catch)
- [turtle](http://turtle.sourceforge.net/)


dependencies
============

- [cmake](http://cmake.org/): only required for building and executing the self-test unit test suite
