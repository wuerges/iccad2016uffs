include(CTest)

find_package(Boost COMPONENTS system filesystem REQUIRED)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

#...
add_executable(TestSpirit1.x test_spirit_1.cxx)
add_executable(TestParser1.x test_parser_1.cxx)
add_executable(TestConvert1.x converts_a_verilog_to_a_graph.cxx)

target_link_libraries(TestParser1.x 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

target_link_libraries(TestConvert1.x 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)




# Novos testes devem ser adicionados aqui 
add_test(check_if_spirit_runs TestSpirit1.x)

add_test(check_if_parser_runs TestParser1.x ../testcases/BDD/in_1.v)

add_test(check_if_parser_runs TestConvert1.x ../testcases/BDD/in_1.v)
