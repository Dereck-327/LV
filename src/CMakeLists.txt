set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)

qt_standard_project_setup()

include_directories( ${CMAKE_SOURCE_DIR}/inc/db_tree )

set(app_headers
    ${CMAKE_SOURCE_DIR}/inc/mainwindow.h
    ${CMAKE_SOURCE_DIR}/inc/ccConsole.h
)

set(app_sources
    main.cpp
    mainwindow.cpp
    ccConsole.cpp
)

set(app_ui
    ui/mainwindow.ui
)

set(app_resources
    icons.qrc
)

set(app_other_files
    ${CMAKE_SOURCE_DIR}/release_note.md
)

if(MSVC)
    set(app_win32 WIN32)
    set(app_rc
        images/icon/cc_icon.rc
        ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/windows/qt5.natvis
    )
elseif(APPLE)
    set(app_bundle MACOSX_BUNDLE)
endif()

add_subdirectory( db_tree )

qt_add_executable(${PROJECT_NAME}_app
    ${app_win32}
    ${app_bundle}
    ${app_headers}
    ${app_sources}
    ${app_ui}
    ${app_resources}
    ${app_rc}
    ${app_other_files}
)

target_link_libraries(${PROJECT_NAME}_app
    PRIVATE
        Qt::Core
        Qt::Widgets
        spdlog::spdlog
        Open3D::Open3D
)

target_include_directories(${PROJECT_NAME}_app
    PUBLIC
        ${CMAKE_SOURCE_DIR}/inc
)

install(TARGETS ${PROJECT_NAME}_app
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}_app
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})