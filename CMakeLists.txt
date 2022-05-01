project(project2)
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")

add_executable(phase2
  phase2.cpp
  IR.cpp
  CFG.cpp
  CodeGen.cpp
  Naive.cpp
  IntraBlock.cpp
  Global.cpp
  )

enable_testing()
add_test(NAME 42_naive COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/42.sh naive)
add_test(NAME 42_intra COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/42.sh intra)
add_test(NAME 42_global COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/42.sh global)
add_test(NAME condition_naive COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/condition.sh naive)
add_test(NAME condition_intra COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/condition.sh intra)
add_test(NAME condition_global COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/condition.sh global)
add_test(NAME call_naive COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/call.sh naive)
add_test(NAME call_intra COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/call.sh intra)
add_test(NAME call_global COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/call.sh global)
add_test(NAME factorial_naive COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/factorial.sh naive)
add_test(NAME factorial_intra COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/factorial.sh intra)
add_test(NAME factorial_global COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/factorial.sh global)
add_test(NAME 42_f_naive COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test/42_f.sh naive)
