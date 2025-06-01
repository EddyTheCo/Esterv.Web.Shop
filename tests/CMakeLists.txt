FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.16.0)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

set(test_list object_core_test product_test) 

if(BUILD_SERVER OR BUILD_CLIENT)
	list(APPEND test_list tcp_client_server)
endif(BUILD_SERVER OR BUILD_CLIENT)

foreach(test IN LISTS test_list)
  add_executable(${test} ${test}.cpp)
  target_link_libraries(${test} PRIVATE Core gtest_main)

if(BUILD_SERVER)
  target_link_libraries(${test} PRIVATE TCPServer)
  target_compile_definitions(${test} PRIVATE BUILD_SERVER)
endif(BUILD_SERVER)

if(BUILD_CLIENT)
  target_link_libraries(${test} PRIVATE TCPClient)
  target_compile_definitions(${test} PRIVATE BUILD_CLIENT)
endif(BUILD_CLIENT)

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


