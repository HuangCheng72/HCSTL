cmake_minimum_required(VERSION 3.17)
project(HCSTL)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/HCSTL)  # 指定include目录为HCSTL


add_executable(HCSTL main.cpp HCSTL/vector.h)