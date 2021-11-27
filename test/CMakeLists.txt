include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${GAME_SOURCE_DIR}/src)

add_executable(
    Test
    utils_test.cpp
)
target_link_libraries(
    Test
    game_lib
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(Test) 

add_test(NAME UnitTest COMMAND Test) 

add_custom_command(TARGET Test POST_BUILD COMMAND ctest --output-on-failure)