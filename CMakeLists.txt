cmake_minimum_required(VERSION 3.22)

project(child_lock_state_decision LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(child_lock_state_decision
    src/child_lock_state_decision.cpp
)

target_include_directories(child_lock_state_decision
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(child_lock_state_decision_test
    test/child_lock_state_decision_test.cpp
)

target_link_libraries(child_lock_state_decision_test
    PRIVATE
        child_lock_state_decision
)

enable_testing()

add_test(
    NAME child_lock_state_decision_test
    COMMAND child_lock_state_decision_test
)
