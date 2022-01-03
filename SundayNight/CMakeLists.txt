cmake_minimum_required(VERSION 3.21)
project(Naava_s_final C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Naava_s_final
        AllocatePseudoCode.c
        UnitTestsAllocate.c AllocatePseudoCode.h)
