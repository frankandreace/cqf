cmake_minimum_required(VERSION 3.19)
project(CQF VERSION 0.1.0)

include(CTest)
enable_testing()

add_compile_options(-Wall -O3 -fpermissive) #-Werror

add_subdirectory(cqf)