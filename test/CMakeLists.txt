cmake_minimum_required(VERSION 3.0.0)

add_executable(linked_buffer_test linked_buffer_test.cpp "http_transceiver_1_1_test.cpp")

target_link_libraries(linked_buffer_test
    GTest::gtest
    GTest::gtest_main
)

add_test(linked_buffer_alltest linked_buffer_test)