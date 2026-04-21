include(GoogleTest)

add_custom_target(LV_tests)
set_target_properties(LV_tests PROPERTIES FOLDER tests)

get_target_property(LV_QT_CORE_LOCATION Qt6::Core IMPORTED_LOCATION_RELEASE)
if(NOT LV_QT_CORE_LOCATION)
  get_target_property(LV_QT_CORE_LOCATION Qt6::Core IMPORTED_LOCATION)
endif()
get_filename_component(LV_QT_RUNTIME_DIR "${LV_QT_CORE_LOCATION}" DIRECTORY)

function(lv_add_test TEST_NAME)
  cmake_parse_arguments(LV_TEST "" "WORKING_DIRECTORY" "SOURCES;LIBRARIES;INCLUDE_DIRS;LABELS" ${ARGN})

  add_executable(${TEST_NAME} ${LV_TEST_SOURCES})
  target_link_libraries(${TEST_NAME}
    PRIVATE
      GTest::gtest_main
      ${LV_TEST_LIBRARIES}
  )

  if(LV_TEST_INCLUDE_DIRS)
    target_include_directories(${TEST_NAME}
      PRIVATE
        ${LV_TEST_INCLUDE_DIRS}
    )
  endif()

  set_target_properties(${TEST_NAME} PROPERTIES FOLDER tests)

  gtest_add_tests(
    TARGET ${TEST_NAME}
    TEST_LIST LV_DISCOVERED_TESTS
  )

  if(LV_DISCOVERED_TESTS)
    set_tests_properties(${LV_DISCOVERED_TESTS} PROPERTIES
      ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${LV_QT_RUNTIME_DIR}"
    )

    if(LV_TEST_WORKING_DIRECTORY)
      set_tests_properties(${LV_DISCOVERED_TESTS} PROPERTIES
        WORKING_DIRECTORY ${LV_TEST_WORKING_DIRECTORY}
      )
    endif()

    if(LV_TEST_LABELS)
      set_tests_properties(${LV_DISCOVERED_TESTS} PROPERTIES
        LABELS "${LV_TEST_LABELS}"
      )
    endif()
  endif()

  add_dependencies(LV_tests ${TEST_NAME})
endfunction()

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/CCAppCommon/test ${CMAKE_BINARY_DIR}/tests/CCAppCommon)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/CCPluginAPI/test ${CMAKE_BINARY_DIR}/tests/CCPluginAPI)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/qCC_db/test ${CMAKE_BINARY_DIR}/tests/qCC_db)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/qCC_glWindow/test ${CMAKE_BINARY_DIR}/tests/qCC_glWindow)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/qCC_io/test ${CMAKE_BINARY_DIR}/tests/qCC_io)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/examples/point_cloud_workflow ${CMAKE_BINARY_DIR}/tests/examples/point_cloud_workflow)