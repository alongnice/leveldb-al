find_package(Threads REQUIRED)


file(GLOB TEST_SOURCES "*.cc" "*.cpp")

if(TEST_SOURCES)
    add_executable(leveldb_tests ${TEST_SOURCES})

    target_link_libraries(leveldb_tests
        leveldb
        gtest
        gtest_main
        Threads::Threads
    )

    add_test(NAME leveldb_tests COMMAND leveldb_tests)
endif()
