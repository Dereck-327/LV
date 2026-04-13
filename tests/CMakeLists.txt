add_executable(${PROJECT_NAME}_tests
  main.cpp
)

target_link_libraries(${PROJECT_NAME}_tests
  # ${PROJECT_NAME}
  # GTest::gtest_main  # 这个也会提供 main函数 可以去掉 使用自定义的main函数
  GTest::gtest
  # log_lib
  # utils_lib
  spdlog::spdlog
)

target_include_directories(${PROJECT_NAME}_tests PUBLIC
  ${CMAKE_SOURCE_DIR}/inc
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)