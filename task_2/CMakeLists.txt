project(task_2 C)

set(CMAKE_C_STANDARD_REQUIRED 99)

include_directories(src)

file(GLOB SOURCES src/*.c)

add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_options(${PROJECT_NAME} PUBLIC -Wall)
