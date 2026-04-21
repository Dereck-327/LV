add_executable(LV_app)

add_subdirectory(include)
add_subdirectory(src)

target_link_libraries(LV_app
  PRIVATE
    CCAppCommon
    CCPluginAPI
    QCC_DB_LIB
    QCC_IO_LIB
    QCC_GL_LIB
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

set_target_properties(LV_app PROPERTIES
  OUTPUT_NAME "LV_app"
)

install(TARGETS LV_app
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  BUNDLE DESTINATION .
)