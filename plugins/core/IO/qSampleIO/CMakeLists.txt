option(PLUGIN_IO_QSAMPLE_IO "Build sample IO plugin" ON)

if(PLUGIN_IO_QSAMPLE_IO)
  project(QSAMPLE_IO_PLUGIN LANGUAGES CXX)

  AddPlugin(NAME ${PROJECT_NAME} TYPE io)

  add_subdirectory(include)
  add_subdirectory(src)
endif()