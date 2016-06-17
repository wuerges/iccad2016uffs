include(CTest)

find_package(Boost COMPONENTS system filesystem REQUIRED)
IF (Boost_FOUND)
  INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
  ADD_DEFINITIONS( "-DHAS_BOOST" )
ENDIF()

#...
add_executable(TestSpirit1.x test_spirit_1.cxx)
add_executable(TestParser1.x test_parser_1.cxx)
add_executable(TestSimulateGraph.x test_simulate_graph.cxx)
add_executable(TestSimulateIcarus.x test_simulate_icarus.cxx)
add_executable(TestConvert1.x converts_a_verilog_to_a_graph.cxx)
add_executable(simulation.x simulation.cxx)

target_link_libraries(TestSimulateIcarus.x 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  verilog_model
)

target_link_libraries(TestSimulateGraph.x 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  verilog_model
)

target_link_libraries(TestParser1.x 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  verilog_model
)

target_link_libraries(TestConvert1.x 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  verilog_model
)

target_link_libraries(simulation.x 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  verilog_model
)


# Novos testes devem ser adicionados aqui 

add_test(check_if_spirit_runs TestSpirit1.x)
add_test(check_if_converter_runs TestConvert1.x ../testcases/BDD/in_1.v)


# Novos testes devem ser adicionados aqui 
FOREACH(i 
./testcases/trivial/buf1.v
./testcases/trivial/or1.v
./testcases/trivial/and1.v
./testcases/trivial/not1.v
./testcases/trivial/nand1.v
./testcases/BDD/in_1.v
./testcases/BDD/in_2.v
./testcases/BDD_wrong/in_1.v
./testcases/BDD_wrong/in_2.v
./testcases/cad16_np3_case/case0/cir2.v
./testcases/cad16_np3_case/case0/cir1.v
./testcases/cad16_np3_case/case1/cir2.v
./testcases/cad16_np3_case/case1/cir1.v
./testcases/cad16_np3_case/case2/cir2.v
./testcases/cad16_np3_case/case2/cir1.v
./testcases/cad16_np3_case/case3/cir2.v
./testcases/cad16_np3_case/case3/cir1.v
./testcases/cad16_np3_case/case4/cir2.v
./testcases/cad16_np3_case/case4/cir1.v
./testcases/cad16_np3_case/case5/cir2.v
./testcases/cad16_np3_case/case5/cir1.v
./testcases/cad16_np3_case2/case6/cir2.v
./testcases/cad16_np3_case2/case6/cir1.v
./testcases/cad16_np3_case2/case7/cir2.v
./testcases/cad16_np3_case2/case7/cir1.v
./testcases/cad16_np3_case2/case8/cir2.v
./testcases/cad16_np3_case2/case8/cir1.v
./testcases/cad16_np3_case2/case9/cir2.v
./testcases/cad16_np3_case2/case9/cir1.v
./testcases/cad16_np3_case2/case10/cir2.v
./testcases/cad16_np3_case2/case10/cir1.v
./testcases/cad16_np3_case2/case11/cir2.v
./testcases/cad16_np3_case2/case11/cir1.v
./testcases/cad16_np3_case2/case12/cir2.v
./testcases/cad16_np3_case2/case12/cir1.v
./testcases/cad16_np3_case2/case13/cir2.v
./testcases/cad16_np3_case2/case13/cir1.v
./testcases/cad16_np3_case2/case14/cir2.v
./testcases/cad16_np3_case2/case14/cir1.v
./testcases/unit1/in_1.v
./testcases/unit1/in_2.v
./testcases/unit2/in_1.v
./testcases/unit2/in_2.v
./testcases/unit3/in_1.v
./testcases/unit3/in_2.v
./testcases/unit4/in_1.v
./testcases/unit4/in_2.v
./testcases/unit5/in_1.v
./testcases/unit5/in_2.v
./testcases/unit6_removed/in_1.v
./testcases/unit6_removed/in_2.v
./testcases/unit7/in_1.v
./testcases/unit7/in_2.v
./testcases/unit8/in_1.v
./testcases/unit8/in_2.v
./testcases/unit9/in_1.v
./testcases/unit9/in_2.v
./testcases/unit10/in_1.v
./testcases/unit10/in_2.v
./testcases/unit11/in_1.v
./testcases/unit11/in_2.v
./testcases/unit12/in_1.v
./testcases/unit12/in_2.v
./testcases/unit13/in_1.v
./testcases/unit13/in_2.v
./testcases/unit14/in_1.v
./testcases/unit14/in_2.v
./testcases/unit15/in_2.v
./testcases/unit15/in_1.v
./testcases/unit16/in_1.v
./testcases/unit16/in_2.v
./testcases/unit17/in_1.v
./testcases/unit17/in_2.v
)
  add_test(check_if_simulator_runs_${i} TestSimulateGraph.x ../${i})
  #add_test(check_if_parser_runs_${i} TestParser1.x ../${i})
ENDFOREACH()



