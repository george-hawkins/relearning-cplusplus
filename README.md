NOTES
=====

Ubuntu 20.04 LTS comes with g++ 9.x. To install g++ 10.x:

    $ sudo apt install g++-10

And in CLion go to _Settings / Build, Execution, Deployment / CMake_ and set _CMake options_ to `-DCMAKE_CXX_COMPILER=g++-10`.

Note: there's various opinions on how you should tell CMake to use a different compiler - see this [SO question](https://stackoverflow.com/q/17275348/245602) for more details.

The `.idea` directory comes with a default `.gitignore` file that ignores `workspace.xml`. Settings like _CMake options_ end up in this file so they do not end up stored in your git repo.

For more on how to work with even more recent versions of g++ see [`docker-cplusplus-coroutines`](https://github.com/george-hawkins/docker-cplusplus-coroutines).

TODO:

Look at `clang-tidy` for static analysis.


