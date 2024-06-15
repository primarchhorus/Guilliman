cmake_minimum_required(VERSION 3.0.0)
project(JobSystem VERSION 0.1.0 LANGUAGES C CXX)
include_directories(include)
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_executable(JobSystem job_system.cpp main.cpp)
set_property(TARGET JobSystem PROPERTY CXX_STANDARD 17)
include(CTest)
enable_testing()

add_executable(
  circular_buffer_test
  circular_buffer_test.cpp
)
target_link_libraries(
  circular_buffer_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(circular_buffer_test)

