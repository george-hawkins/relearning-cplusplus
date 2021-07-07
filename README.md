Relearning C++
==============

Ubuntu 20.04 LTS comes with g++ 9.x. To install g++ 10.x:

    $ sudo apt install g++-10

And in CLion go to _Settings / Build, Execution, Deployment / CMake_ and set _CMake options_ to `-DCMAKE_CXX_COMPILER=g++-10`.

Note: there's various opinions on how you should tell CMake to use a different compiler - see this [SO question](https://stackoverflow.com/q/17275348/245602) for more details.

The `.idea` directory comes with a default `.gitignore` file that ignores `workspace.xml`. Settings like _CMake options_ end up in this file so they do not end up stored in your git repo.

For more on how to work with even more recent versions of g++ see [`docker-cplusplus-coroutines`](https://github.com/george-hawkins/docker-cplusplus-coroutines).

TODO:

Look at `clang-tidy` for static analysis.

CMake etc.
----------

CLion creates `CMakeLists.txt` files that require a more recent version of CMake than that installed by `apt` on Ubuntu 20.04 (to get more recent versions, you need to use `snap`).

So to use the versions of CMake and other tools that come bundled with CLion:

    $ mkdir ~/idea-bin

Copy this script into `~/idea-bin/clion-path`:

```bash
# Source this file to add the CLion executables for CMake, GDB, LLDB and Clang to your path.

# This file needs to be in the same directory as the `clion` shell script.
# This shell script is created if you enable shell scripts in Toolbox and specify e.g. `~/idea-bin` as the location for them.

dir=$(dirname "$0")

root=$(sed -n 's?"\(.*\)/clion.sh".*?\1?p' $dir/clion)

# CMake etc. have a linux/bin directory whereas Clang just has a linux directory.
PATH=$root:$root/cmake/linux/bin:$root/gdb/linux/bin:$root/lldb/linux/bin:$root/clang/linux
```

As noted in the `#` comments above, you need to enable shell scripts in Toolbox as described.

Then you can add the necessary entries to `PATH` with:

    $ source ~/idea-bin/clion-path

Notes from elsewhere
--------------------

https://docs.microsoft.com/en-us/cpp/cpp/welcome-back-to-cpp-modern-cpp

https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines
https://github.com/microsoft/GSL

It seems easy to include GSL in your projects - see using CMake `FetchContent` [here](https://github.com/microsoft/GSL#usage-in-cmake).

Other libraries provide similar `FetchContent` instructions, e.g. [here](https://google.github.io/googletest/quickstart-cmake.html) for GoogleTest.

Oddly, the GSL is barely documented - the only thing seems to be this [short section](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-gsl) in the Core Guidelines. It doesn't look as if MS plan to address this - see issue [#354](https://github.com/microsoft/GSL/issues/354). 

The best thing seems to be just to search the Guidelines (it's a single page) for usage examples, e.g. just search for `gsl::span`. It looks like you're supposed to discover things via the Guidelines rather than by examining the library and its non-existant documentation.

For a very brief overview, see these [notes](http://www.modernescpp.com/index.php/c-core-guideline-the-guidelines-support-library) by Rainer Grimm (start at "You may be wondering that the GSL has its own smart pointer").

Interesting, if you look at the GSL README (above) and this SO answer, it seems CMake has dependency-management-like features:
https://stackoverflow.com/a/1136645/245602

Conan - a C++ package manager: https://conan.io/

C++ libraries of interest (includes Boost.ASIO mentioned by Vlad):
https://stackoverflow.com/a/782146/245602

Surpringly little out there for modern C++ book-wise. This seems to be the one:
https://www.amazon.co.uk/Tour-C-Depth/dp/0134997832/

Possibles:

* https://www.amazon.de/gp/product/B08GM9KKMR/
* https://www.amazon.de/dp/1788993861/

For a list of a comprehensive tooling setup (that corelates well with the impressions I've been forming so far) see the first post in this Reddit [thread](https://www.reddit.com/r/cpp/comments/af74l1/recommendations_for_setting_up_a_modern_c_dev/). Covers Ninja, `clang-tidy`, sanitizers etc.

For examples to test the built-in CLion [`clang-tidy` support](https://www.jetbrains.com/help/clion/clang-tidy-checks-support.html), see these [MS examples](https://docs.microsoft.com/en-us/cpp/code-quality/using-the-cpp-core-guidelines-checkers?view=msvc-160#examples) for their C++ Core Guidelines checker.

Note: the MS C++ Core Guidelines checker is only available in tools like VS Code, `clang-tidy` seems the most similar tool available on Linux.

The `clang-tidy` [documentation](https://clang.llvm.org/extra/clang-tidy/) (ignore the line about setting up "a compile command database for your project"). And ignore any references to `clang-check` elsewhere - just use `clang-tidy`.
