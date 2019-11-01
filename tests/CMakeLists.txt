enable_testing()

find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED)
pkg_check_modules(GMOCK REQUIRED)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
)

add_executable(tests
    tests.cpp
)

target_link_libraries(tests
    gtest_main
    gtest
    gmock
    pthread
)

add_test(
    NAME tests
    COMMAND tests
)
