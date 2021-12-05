cmake_minimum_required(VERSION 3.10)
project(pa3)

# src file globbing
file(GLOB_RECURSE sources src/*.c)

# executable
add_executable(pa3 ${sources})

# include directories
target_include_directories(pa3 PUBLIC inc)

# compiler flags
target_compile_options(pa3 PUBLIC -std=c99 -pedantic -Wvla -Wall -Wshadow -g)
