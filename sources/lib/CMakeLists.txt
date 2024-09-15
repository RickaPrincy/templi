project(
  ${TEMPLI_LIB_NAME}
  VERSION 1.0.0
  LANGUAGES CXX)

set(LIB_SOURCES fs_utils.cpp json_config.cpp parser.cpp templi.cpp types.cpp
                utils.cpp)

# Create the library
if(BUILD_SHARED_LIBS)
  add_library(${TEMPLI_LIB_NAME} SHARED ${LIB_SOURCES})
else()
  add_library(${TEMPLI_LIB_NAME} STATIC ${LIB_SOURCES})
endif()

# Set include directories for the library
target_include_directories(
  ${TEMPLI_LIB_NAME} PRIVATE $<BUILD_INTERFACE:${TEMPLI_SOURCE_DIR}/include>)

target_link_libraries(${TEMPLI_LIB_NAME} PUBLIC rcli)

# set properties
set_target_properties(
  ${TEMPLI_LIB_NAME}
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             SOVERSION ${TEMPLI_VERSION_MAJOR}
             VERSION ${TEMPLI_VERSION})

# lib install configuration
if(TEMPLI_LIB_INSTALL)
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/${TEMPLI_LIB_NAME}Version.cmake
    VERSION ${TEMPLI_VERSION}
    COMPATIBILITY SameMajorVersion)

  include(GNUInstallDirs)
  install(DIRECTORY ${TEMPLI_SOURCE_DIR}/include/${TEMPLI_LIB_NAME}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  install(
    TARGETS ${TEMPLI_LIB_NAME}
    EXPORT ${TEMPLI_LIB_NAME}Targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Development)

  # Install the Config and ConfigVersion files
  install(
    EXPORT ${TEMPLI_LIB_NAME}Targets
    FILE ${TEMPLI_LIB_NAME}Targets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${TEMPLI_LIB_NAME})

  configure_file(${TEMPLI_SOURCE_DIR}/cmake/${TEMPLI_LIB_NAME}Config.cmake.in
                 ${CMAKE_BINARY_DIR}/cmake/${TEMPLI_LIB_NAME}Config.cmake @ONLY)
  # Install the Config, ConfigVersion, and custom Find module files
  install(FILES ${CMAKE_BINARY_DIR}/cmake/${TEMPLI_LIB_NAME}Config.cmake
                ${CMAKE_BINARY_DIR}/cmake/${TEMPLI_LIB_NAME}Version.cmake
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${TEMPLI_LIB_NAME})
endif()
