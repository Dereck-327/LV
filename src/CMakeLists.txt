set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

qt_standard_project_setup()


qt_add_executable(${PROJECT_NAME}_app
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    ${CMAKE_SOURCE_DIR}/inc/mainwindow.h
    ui/mainwindow.ui
)

qt_add_translations(
    TARGETS ${PROJECT_NAME}_app
    TS_FILES ${CMAKE_SOURCE_DIR}/LV_zh_CN.ts
)

target_link_libraries(${PROJECT_NAME}_app
    PRIVATE
        Qt::Core
        Qt::Widgets
        spdlog::spdlog
        Open3D::Open3D
)


target_include_directories(${PROJECT_NAME}_app PUBLIC
  ${CMAKE_SOURCE_DIR}/inc
   
)

install(TARGETS ${PROJECT_NAME}_app
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}_app
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

