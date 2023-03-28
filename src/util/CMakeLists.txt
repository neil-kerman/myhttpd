cmake_minimum_required(VERSION 3.18)

if(BUILD_UNIT_TESTS)
    add_executable(linked_buffer_test
        "linked_buffer_test.cpp"
    )
    target_link_libraries(linked_buffer_test
        GTest::gtest
        GTest::gtest_main
    )

    add_test(LINKED_BUFFER_TEST linked_buffer_test)
endif()