cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME LV)

project(${PROJECT_NAME} LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)




set(ENABLE_UNIT_TESTS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/third_party)

# 拉取 GoogleTest
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# 拉取 Eigen
FetchContent_Declare(
  eigen
  URL https://gitlab.com/libeigen/eigen/-/archive/3.4.1/eigen-3.4.1.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# 拉取spdlog
FetchContent_Declare(
  spdlog
  URL https://github.com/gabime/spdlog/archive/refs/tags/v1.17.0.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# 下载并添加到构建
FetchContent_MakeAvailable(googletest eigen spdlog)


add_subdirectory(src)

if(ENABLE_UNIT_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()
