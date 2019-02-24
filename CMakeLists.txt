cmake_minimum_required(VERSION 3.9.6)

# Подключаем поддержку потоков, т.к. она нужна для тестирования
find_package(Threads REQUIRED)

# Настраиваем зависимость от Google Test Framework (GTest и GMock)
# Он будет скачан и его не потребуется ставить руками в системе
include(ExternalProject)

ExternalProject_Add(
    gtest
    URL https://github.com/google/googletest/archive/master.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
    # Отменяем установку
    INSTALL_COMMAND ""
)

# Получаем пути к исходникам и бинарникам GTest
ExternalProject_Get_Property(gtest source_dir binary_dir)

# Указываем что надо собрать библиотеку 'libgtest', которую потом будем подключать к тестам
add_library(libgtest IMPORTED STATIC GLOBAL)
add_dependencies(libgtest gtest)

# Настраиваем свойства для библиотеки 'libgtest'
set_target_properties(libgtest PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/lib/libgtest.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Проделываем тоже самое с GMock, чтобы получить библиотеку 'libgmock'
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gtest)

# Настраиваем свойства для библиотеки 'libgmock'
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/lib/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)

# Указываем папки где следует искать заголовки проектов
include_directories("${source_dir}/googletest/include"
                    "${source_dir}/googlemock/include")

# Разрешаем тесты
enable_testing()

# Добавляем подпроекты с задачами из тестового задания
add_subdirectory(task_1)
add_subdirectory(task_2)
