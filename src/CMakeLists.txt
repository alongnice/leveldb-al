# 列出实际存在的源文件（暂时我们就放个占位）
add_library(leveldb
    util/slice.cc
    util/status.cc
)

target_include_directories(leveldb PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/leveldb
)

find_package(Threads REQUIRED)
target_link_libraries(leveldb PUBLIC Threads::Threads)
