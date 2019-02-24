project(task_1 CXX)

set(CMAKE_CXX_STANDARD_REQUIRED 14)

include_directories(src)

file(GLOB TASK_1_SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${TASK_1_SOURCES})
target_compile_options(${PROJECT_NAME} PUBLIC -Wall)

enable_testing()
add_subdirectory(test)
