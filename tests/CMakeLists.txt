
enable_testing()
add_definitions(-std=c++17 -O3)

find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED)

include_directories(
    ${GTEST_INCLUDE_DIRS}
)

add_executable(tests
    tests.cpp
)

target_link_libraries(tests
    gtest_main
    gtest
    gmock
)

add_test(
    NAME tests
    COMMAND tests
)
