Temporary output folder
=======================

Although it is best to avoid writing tests which creates files in the local filesystem, but nevertheless there are certain cases in which it could come in handy to be able to dump out some temp files into the file system during testing.

`cute` supports this usage scenario by providing the function `cute::temp_folder()` which can be called from within a test case implementation. The function creates a new and empty folder and returns the path to it. This ensures that each test case have its own temp folder and does not write into a shared location, which could lead to all kinds of trouble during testing. After finishing the test case execution the created temp folder will be cleaned up automatically again.

On the `Windows` platform the temp folder cleanup step will additionally verify that all files created in the test temp folder are really deleted again (i.e., that there are no pending file locks on them anymore) and report a test failure otherwise. Due to some differences in file handling and file locking this extra validation step does not work on non-`Windows` platforms.

```C++
CUTE_TEST("The temp folder should be deleted after a test case") {
    auto folder = cute::temp_folder();

    // create some files in the temp folder
    std::ofstream(folder + "file_1");
    std::ofstream(folder + "file_2");
    std::ofstream(folder + "file_3");

    // we cannot have a test case without an actual check/assert in it => add a dummy check
    CUTE_ASSERT(true);
}
```
All created files get deleted automatically again.

The following test will fail under `Windows` since it holds on the file lock even beyond the end of the test case.
```C++
CUTE_TEST("A test should fail if the temp folder could not be deleted afterwards (Windows only)") {
    auto folder = cute::temp_folder();

    // static => keep a file lock around even after the test
    // in order to verify that the test fails if the temp folder
    // could not completely cleaned up again
    static auto keep_alive = std::make_shared<std::ofstream>(folder + "file_1");

    // we cannot have a test case without an actual check/assert in it => add a dummy check
    CUTE_ASSERT(true);
}
```
The output will look like this:
```
.../cute_tests.cpp:10: error:  test should fail if the temp folder could not be deleted afterwards (Windows only)
.../cute_tests.cpp:10: error:     duration:   0 ms
.../cute_tests.cpp:10: error:     reason:     could not cleanup temp folder
```
