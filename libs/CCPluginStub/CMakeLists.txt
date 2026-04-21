project(CCPluginStub LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC)

add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Qt6::Core
)

InstallSharedLibrary(TARGET ${PROJECT_NAME})