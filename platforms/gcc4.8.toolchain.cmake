# http://stackoverflow.com/questions/7031126/switching-between-gcc-and-clang-llvm-using-cmake

set( ClangRoot, /home/zaqwes/opt/gcc4.8-clang-toolchains/bin)

set(CMAKE_C_COMPILER    /home/zaqwes/opt/gcc4.8-clang-toolchains/bin/gcc)
set(CMAKE_CXX_COMPILER  /home/zaqwes/opt/gcc4.8-clang-toolchains/bin/g++)

#SET (CMAKE_C_FLAGS_INIT                "-Wall -std=c99")
#SET (CMAKE_C_FLAGS_DEBUG_INIT          "-g")
#SET (CMAKE_C_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")

#SET (CMAKE_CXX_FLAGS_INIT                "-Wall")
#SET (CMAKE_CXX_FLAGS_DEBUG_INIT          "-g")
#SET (CMAKE_CXX_FLAGS_MINSIZEREL_INIT     "-Os -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELEASE_INIT        "-O4 -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO_INIT "-O2 -g")