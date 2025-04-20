foreach(test object_core_test)
  add_executable(${test} ${test}.cpp)
  target_link_libraries(${test} PRIVATE Core)
  add_test(NAME ${test} COMMAND ${test})
  if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
      TARGET ${test}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${test}> $<TARGET_FILE_DIR:${test}>
      COMMAND_EXPAND_LISTS)
  endif(WIN32 AND BUILD_SHARED_LIBS)
endforeach()
