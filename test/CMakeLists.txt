# This file describes the testcases

include(CTest)
add_executable(TestSpirit1.x test_spirit_1.cxx)
add_test(check_if_spirit_runs TestSpirit1)
