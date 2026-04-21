option(PLUGIN_STANDARD_QSAMPLE_STANDARD "Build sample standard plugin" ON)

if(PLUGIN_STANDARD_QSAMPLE_STANDARD)
  project(QSAMPLE_STANDARD_PLUGIN LANGUAGES CXX)

  AddPlugin(NAME ${PROJECT_NAME})

  add_subdirectory(include)
  add_subdirectory(src)
endif()