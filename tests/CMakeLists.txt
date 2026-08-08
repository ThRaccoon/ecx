add_executable(tests
    "stub.cpp"
    "containers/sparse_set_tests.cpp"
)

target_link_libraries(tests PRIVATE ecx)

enable_testing()

find_package(GTest CONFIG REQUIRED)
target_link_libraries(tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

add_test(ecx_tests tests)
