project(test_task_1 CXX)

set(CMAKE_CXX_FLAGS -DUNIT_TEST)

set(TEST_TASK_1_LEXER_SOURCES test_lexer.cpp)
add_executable(test_task_1_lexer
  ${TASK_1_SOURCES}
  ${TEST_TASK_1_LEXER_SOURCES})
add_test(NAME test_task_1_lexer
  COMMAND test_task_1_lexer)

set(TEST_TASK_1_PARSER_SOURCES test_parser.cpp)
add_executable(test_task_1_parser
  ${TASK_1_SOURCES}
  ${TEST_TASK_1_PARSER_SOURCES})
target_link_libraries(test_task_1_parser
  libgtest)
add_test(NAME test_task_1_parser
  COMMAND test_task_1_parser)

set(TEST_TASK_1_SOLVER_SOURCES test_solver.cpp)
add_executable(test_task_1_solver
  ${TASK_1_SOURCES}
  ${TEST_TASK_1_SOLVER_SOURCES})
target_link_libraries(test_task_1_solver
  libgtest)
add_test(NAME test_task_1_solver
  COMMAND test_task_1_solver)
