set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
cmake_minimum_required(VERSION 3.19)
set(PROJECT_NAME LV)

project(${PROJECT_NAME} LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)




set(ENABLE_UNIT_TESTS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "Install path prefix." FORCE)

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

# ===== Open3D (C++ minimal) =====
# 仅保留 C++ 基础能力，关闭 Python/样例/测试/基准等
set(BUILD_PYTHON_MODULE OFF CACHE BOOL "" FORCE)
set(BUILD_JUPYTER_EXTENSION OFF CACHE BOOL "" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_BENCHMARKS OFF CACHE BOOL "" FORCE)

# 关闭可选硬件/网络/加速模块，减少依赖链
set(BUILD_WEBRTC OFF CACHE BOOL "" FORCE)
set(BUILD_AZURE_KINECT OFF CACHE BOOL "" FORCE)
set(BUILD_LIBREALSENSE OFF CACHE BOOL "" FORCE)
set(BUILD_CUDA_MODULE OFF CACHE BOOL "" FORCE)
set(BUILD_SYCL_MODULE OFF CACHE BOOL "" FORCE)
set(BUILD_ISPC_MODULE OFF CACHE BOOL "" FORCE)
set(ENABLE_ISPC_MODULE OFF CACHE BOOL "" FORCE)

# 要用 Open3D 新版 GUI 控件，再改成 ON
set(BUILD_GUI OFF CACHE BOOL "" FORCE)



FetchContent_Declare(
  open3d
  GIT_REPOSITORY https://github.com/isl-org/Open3D.git
  GIT_TAG v0.19.0
  GIT_SHALLOW TRUE
)


# 下载并添加到构建
FetchContent_MakeAvailable(googletest eigen spdlog open3d)


add_subdirectory(src)

if(ENABLE_UNIT_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

install(DIRECTORY "${CMAKE_SOURCE_DIR}/build/install" DESTINATION inc)

