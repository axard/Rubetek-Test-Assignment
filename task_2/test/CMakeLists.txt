project(test_task_2 CXX)

add_definitions(-DHEAP_SIZE=1024)

set(TEST_TASK_2_DMM_SOURCES test_dmm.cpp)
add_executable(test_task_2_dmm
  ${TEST_TASK_2_DMM_SOURCES})
target_link_libraries(test_task_2_dmm
  libgtest
  dmm)
add_test(NAME test_task_2_dmm
  COMMAND test_task_2_dmm)
