cmake_minimum_required(VERSION 3.24)

project(LV VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
  "${CMAKE_CURRENT_SOURCE_DIR}/plugins/cmake"
)

include(CTest)
include(FetchContent)
include(GNUInstallDirs)
include(LVProjectOptions)
include(LVDependencies)
include(InstallHelpers)
include(Plugins)

lv_setup_project_options()
lv_configure_dependencies()

add_subdirectory(libs)
add_subdirectory(qCC)

if(LV_BUILD_PLUGINS)
  add_subdirectory(plugins)

  set(LV_PLUGIN_TARGETS ${CC_PLUGIN_TARGET_LIST})
  list(FILTER LV_PLUGIN_TARGETS EXCLUDE REGEX "^$")
  if(LV_PLUGIN_TARGETS)
    add_custom_target(LV_plugins DEPENDS ${LV_PLUGIN_TARGETS})
    add_dependencies(LV_app LV_plugins)
  endif()
endif()

if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()

