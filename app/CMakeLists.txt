
find_package(Boost COMPONENTS system filesystem REQUIRED)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

add_library(verilog_parser ../lib/verilog_parser.cxx ../lib/verilog_model.cxx)

add_executable(TestParser_fields.x test_parser_fields.cxx)
add_executable(iccad2016solve.x iccad2016solve.cxx)

target_link_libraries(iccad2016solve.x
  verilog_parser
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

target_link_libraries(TestParser_fields.x 
  verilog_parser
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

