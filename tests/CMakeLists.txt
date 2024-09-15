file(
  GLOB_RECURSE TEST_SOURCES
  LIST_DIRECTORIES false
  *.h *.cpp)
add_executable(${TEMPLI_TEST_NAME} ${TEST_SOURCES})

target_include_directories(
  ${TEMPLI_TEST_NAME}
  PRIVATE "${TEMPLI_SOURCE_DIR}/include"
  PRIVATE "${TEMPLI_SOURCE_DIR}/sources/lib"
  PRIVATE "${TEMPLI_SOURCE_DIR}/external/googletest/include")

target_link_directories(${TEMPLI_TEST_NAME} PUBLIC ${CMAKE_BINARY_DIR}/lib)

target_link_libraries(
  ${TEMPLI_TEST_NAME}
  PUBLIC ${TEMPLI_LIB_NAME}
  PUBLIC gtest)

set_target_properties(${TEMPLI_TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                     "${CMAKE_BINARY_DIR}/bin")
