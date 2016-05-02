# This file describes the testcases

include(CTest)

find_package(Boost COMPONENTS system filesystem REQUIRED)
#...
add_executable(TestSpirit1.x test_spirit_1.cxx)
add_executable(TestParser1.x test_parser_1.cxx)

target_link_libraries(TestParser1.x 
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)

add_test(check_if_spirit_runs TestSpirit1.x)
add_test(check_if_parser_runs TestParser1.x ../testcases/BDD/in_1.v)
