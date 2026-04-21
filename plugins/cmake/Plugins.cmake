set(CC_PLUGIN_TARGET_LIST "" CACHE INTERNAL "LV plugin target list")

function(AddPlugin)
  cmake_parse_arguments(ADD_PLUGIN "" "NAME;TYPE;SHADER_FOLDER" "" ${ARGN})

  if(NOT ADD_PLUGIN_NAME)
    message(FATAL_ERROR "AddPlugin requires NAME")
  endif()

  if(NOT ADD_PLUGIN_TYPE)
    set(ADD_PLUGIN_TYPE standard)
  endif()

  set(PLUGIN_TARGET "${ADD_PLUGIN_NAME}")
  add_library(${PLUGIN_TARGET} MODULE)

  target_link_libraries(${PLUGIN_TARGET}
    PRIVATE
      CCPluginAPI
      CCAppCommon
      QCC_DB_LIB
      QCC_IO_LIB
      QCC_GL_LIB
      Qt6::Core
  )

  set_target_properties(${PLUGIN_TARGET} PROPERTIES
    PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/plugins"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  )

  set_property(TARGET ${PLUGIN_TARGET} PROPERTY PLUGIN_TYPE ${ADD_PLUGIN_TYPE})
  set(CC_PLUGIN_TARGET_LIST "${CC_PLUGIN_TARGET_LIST};${PLUGIN_TARGET}" CACHE INTERNAL "LV plugin target list" FORCE)

  if(ADD_PLUGIN_TYPE STREQUAL "gl" AND ADD_PLUGIN_SHADER_FOLDER)
    add_custom_command(TARGET ${PLUGIN_TARGET} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/${ADD_PLUGIN_SHADER_FOLDER}"
        "$<TARGET_FILE_DIR:${PLUGIN_TARGET}>/${ADD_PLUGIN_SHADER_FOLDER}"
    )
  endif()

  InstallModuleLibrary(TARGET ${PLUGIN_TARGET} DESTINATION "${CMAKE_INSTALL_BINDIR}/plugins")
  message(STATUS "Added ${ADD_PLUGIN_TYPE} plugin: ${PLUGIN_TARGET}")
endfunction()