project(task_2 C)

set(CMAKE_C_STANDARD_REQUIRED 99)

include_directories(src)

file(GLOB TASK_2_SOURCES src/*.c)

# Оформляем задание как библиотеку, которая будет линковаться статически
# Dynamic Memory Manager
add_library(dmm STATIC ${TASK_2_SOURCES})

enable_testing()
add_subdirectory(test)
