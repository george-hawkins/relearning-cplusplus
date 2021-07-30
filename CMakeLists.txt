cmake_minimum_required(VERSION 3.19)
project(relearning_cplusplus)

# The compiler is set via _Cmake options_ in the IntelliJ _Settings_.
# Attempting to set it here results in bad things TM.
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++-12")

# At the moment libc++ doesn't support `ranges` - there's partial support in 13 (whereas libstdc++ already has full support in 10).
#if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#    add_compile_options(-stdlib=libc++)
#    add_link_options(-stdlib=libc++)   # New command on CMake master, not in 3.12 release
#endif()

set(CMAKE_CXX_STANDARD 20)

# See tour_test.cpp.
add_compile_definitions("_LIBCPP_DEBUG=1")
add_compile_definitions("_GLIBCXX_DEBUG")

set(CMAKE_REQUIRED_FLAGS "-fcoroutines-ts")

# ----------------------------------------------------------------------

# Copied from https://google.github.io/googletest/quickstart-cmake.html

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/e2239ee6043f73722e7aa812a459f54a28552929.zip # v1.11.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(hello_test hello_test.cpp)
target_link_libraries(hello_test gtest_main)

include(GoogleTest)
gtest_discover_tests(hello_test)

# ----------------------------------------------------------------------

# Copied from https://github.com/microsoft/GSL

# NOTE: This example uses cmake version 3.14 (FetchContent_MakeAvailable).
# Since it streamlines the FetchContent process
cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# In this example we are picking a specific tag.
# You can also pick a specific commit, if you need to.
FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v3.1.0"
)

FetchContent_MakeAvailable(GSL)

# ----------------------------------------------------------------------

# Not coped from anywhere - just followed the patterns above.

# Only needed if using libstdc++ < 11.2.0. C++20 support was added in 11.2.0:
# https://github.com/gcc-mirror/gcc/commit/03d5044b31f7bf94fcda4136b4ed87a5fee7735d

FetchContent_Declare(date
    GIT_REPOSITORY "https://github.com/HowardHinnant/date"
    GIT_TAG "v3.0.1"
)

FetchContent_MakeAvailable(date)

# ----------------------------------------------------------------------

# Coroutine support is sort of there in Clang but it doesn't currently play well with GNU's libstdc++.
#FetchContent_Declare(cppcoro
#    GIT_REPOSITORY "https://github.com/andreasbuhr/cppcoro"
#    GIT_TAG 7cc9433436fe8f2482138019cfaafce8e1d7a896
#)
#
#FetchContent_MakeAvailable(cppcoro)

# ----------------------------------------------------------------------

add_executable(tour_test tour_test.cpp)
target_link_libraries(tour_test
    PRIVATE gtest_main
    PRIVATE GSL
    PRIVATE date
)

gtest_discover_tests(tour_test)


# ----------------------------------------------------------------------

add_executable(relearning_cplusplus main.cpp Foo.cpp Foo.h)
