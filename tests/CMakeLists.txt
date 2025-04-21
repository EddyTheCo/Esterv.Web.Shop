FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

foreach(test object_core_test product_test)
  add_executable(${test} ${test}.cpp)
  target_link_libraries(${test} PRIVATE Core gtest_main)
  gtest_discover_tests(${test})
  if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
      TARGET ${test}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${test}> $<TARGET_FILE_DIR:${test}>
      COMMAND_EXPAND_LISTS)
  endif(WIN32 AND BUILD_SHARED_LIBS)
endforeach()
