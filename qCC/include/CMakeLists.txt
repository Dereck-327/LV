target_sources(LV_app
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/MainWindow.h
    ${CMAKE_CURRENT_LIST_DIR}/ccConsole.h
    ${CMAKE_CURRENT_LIST_DIR}/ccDBRoot.h
)

target_include_directories(LV_app
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)