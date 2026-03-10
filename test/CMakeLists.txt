# Testing uses CTest
enable_testing()

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Gather source files
set(TEST_SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/F08_RearRiskProtectionController.c
)

# Register tests
include(GoogleTest)

# F-08 Unit Test Target
add_executable(test_F08_RearRiskProtection
    test_F08_RearRiskProtectionController.cpp
    ${TEST_SRC_FILES}
)
target_link_libraries(test_F08_RearRiskProtection gtest_main)
gtest_discover_tests(test_F08_RearRiskProtection)
