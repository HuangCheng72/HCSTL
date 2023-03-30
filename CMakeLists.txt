cmake_minimum_required(VERSION 3.17)
project(HCSTL)

set(CMAKE_CXX_STANDARD 14)

include_directories(${PROJECT_SOURCE_DIR}/HCSTL)  # 指定include目录为HCSTL


add_executable(HCSTL main.cpp HCSTL/vector.h HCSTL/type_traits.h HCSTL/allocator.h HCSTL/constructor.h HCSTL/list.h HCSTL/iterator.h HCSTL/algorithm.h HCSTL/queue.h HCSTL/functional.h HCSTL/heap.h HCSTL/util.h HCSTL/deque.h HCSTL/uninitialized.h HCSTL/stack.h HCSTL/pair.h)
