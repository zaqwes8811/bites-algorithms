# debig troubles
# http://stackoverflow.com/questions/19912648/cmake-qtcreator-mingw-debug-mode-windows-7
# http://www.cmake.org/Wiki/CMake_Useful_Variables
# -DCMAKE_BUILD_TYPE=Debug
# -DCMAKE_VERBOSE_MAKEFILE=ON -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
#
#
# -DCMAKE_TOOLCHAIN_FILE=/home/zaqwes/work/algorithmic-space/platforms/llvm-clang-cilkplus.toolchain.cmake

project(alg-space)
cmake_minimum_required(VERSION 2.8)

set(Extern "../../extern/")
set(Intern "../../intern/")
set(GMock "../../extern/gmock-1.6.0/fused-src")
set(Source "source/")
set(testsRoot "./tests")
set(TrainRoot "./train")
set(BigExtern "/opt/big-3rdparty/")


# Test framework
include_directories(${GMock})
include_directories(${Source})
include_directories(${Extern}/loki-0.1.7/include)
include_directories(${Extern})
include_directories(${Intern})
include_directories(${BigExtern}/boost_1_57_0)

# link the libraries to the executable
if (MINGW)
    link_directories(${BigExtern}/boost_1_57_0/stage/lib)
    add_definitions(  -Wall -Wno-unused-local-typedefs)# -Wincompatible-pointer-types)
    #add_definitions(-DGTEST_USE_OWN_TR1_TUPLE=1 -DGTEST_HAS_TR1_TUPLE=1)
    #LINK_LIBRARIES(ws2_32 WSnmp32 boost_system-mgw48-mt-s-1_57)

elseif (MSVC90)
    link_directories(${BigExtern}/boost_1_57_0/stage/lib)
    add_definitions( -D_CRT_SECURE_NO_WARNINGS)# /showIncludes)#D)#T)

elseif (UNIX)
    # gcc 4.8 ubuntu 12 http://llvm.org/docs/GettingStarted.html#getting-a-modern-host-c-toolchain
    # cmake .. -DCMAKE_CXX_LINK_FLAGS="-Wl,-rpath,$HOME/toolchains/lib64 -L$HOME/toolchains/lib64"
    # http://llvm.org/docs/GettingStarted.html#getting-a-modern-host-c-toolchain

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        link_directories(${BigExtern}/boost_1_57_0/stage_gcc/lib)
        link_libraries(tbb tbbmalloc pthread rt)  # !! pthread and rt last!!
    else()
        add_definitions(  -Wall )#-std=c++11)
        add_definitions(  -std=c++0x)
        link_directories(${BigExtern}/boost_1_57_0/stage_gcc/lib)
        link_libraries(pthread rt)  # !! pthread and rt last!!
    endif()

endif()

file(GLOB blockedTestSources
    ${testsRoot}/*.cc
    ${testsRoot}/dyn_prog/*.cc
    ${Intern}/details/*.cc
    ${testsRoot}/../../stanford_algoritms_part2/tests/*.cc
)

add_executable(unittest.bin
    ${Source}/mains/launcher_tests.cc
    ${GMock}/gmock-gtest-all.cc
    ${blockedTestSources}
)

