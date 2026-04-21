option(PLUGIN_GL_QSAMPLE_GL "Build sample GL plugin" ON)

if(PLUGIN_GL_QSAMPLE_GL)
  project(QSAMPLE_GL_PLUGIN LANGUAGES CXX)

  AddPlugin(NAME ${PROJECT_NAME} TYPE gl SHADER_FOLDER shaders)

  add_subdirectory(include)
  add_subdirectory(src)
endif()