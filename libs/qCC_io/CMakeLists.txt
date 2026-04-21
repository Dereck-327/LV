project(QCC_IO_LIB LANGUAGES CXX)

if(LV_BUILD_SHARED_CORE_LIBS)
  add_library(${PROJECT_NAME} SHARED)
else()
  add_library(${PROJECT_NAME} STATIC)
endif()

add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    QCC_DB_LIB
    Qt6::Core
)

InstallSharedLibrary(TARGET ${PROJECT_NAME})