project(test_task_1 CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -DTEST)

set(TEST_TASK_1_LEXER_SOURCES test_lexer.cpp)
add_executable(test_task_1_lexer
  ${TASK_1_SOURCES}
  ${TEST_TASK_1_LEXER_SOURCES})
add_test(NAME test_task_1_lexer
  COMMAND test_task_1_lexer)
