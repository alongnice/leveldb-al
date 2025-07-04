cmake_minimum_required(VERSION 3.15)
project(leveldb-al VERSION 0.1 LANGUAGES C CXX)

# c++11版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加 src 目录
add_subdirectory(src)

# 添加 GoogleTest（注意 binary dir 必须显式指定）
add_subdirectory(third_party/googletest ${CMAKE_BINARY_DIR}/googletest-build)

# 添加 tests 目录
enable_testing()
add_subdirectory(tests)
