
find_package(Boost COMPONENTS system filesystem REQUIRED)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

add_executable(TestParser_fields.x test_parser_fields.cxx)

target_link_libraries(TestParser_fields.x 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

