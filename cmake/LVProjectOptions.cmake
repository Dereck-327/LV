function(lv_setup_project_options)
  set(CMAKE_CXX_STANDARD 20 PARENT_SCOPE)
  set(CMAKE_CXX_STANDARD_REQUIRED ON PARENT_SCOPE)
  set(CMAKE_CXX_EXTENSIONS OFF PARENT_SCOPE)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON PARENT_SCOPE)

  option(LV_BUILD_PLUGINS "Build the plugin system and demo plugins" ON)
  option(LV_BUILD_SHARED_CORE_LIBS "Build core LV libraries as shared libraries" OFF)
  option(LV_ENABLE_UNIT_TESTS "Build GoogleTest-based unit tests" ON)

  set(CMAKE_AUTOMOC ON PARENT_SCOPE)
  set(CMAKE_AUTOUIC ON PARENT_SCOPE)
  set(CMAKE_AUTORCC ON PARENT_SCOPE)

  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" PARENT_SCOPE)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" PARENT_SCOPE)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" PARENT_SCOPE)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path prefix." FORCE)

  if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL" CACHE STRING "" FORCE)
    add_compile_options(/W4 /permissive- /Zc:__cplusplus /EHsc)
  else()
    add_compile_options(-Wall -Wextra -Wpedantic)
  endif()

  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endfunction()