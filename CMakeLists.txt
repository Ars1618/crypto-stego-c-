cmake_minimum_required(VERSION 3.10)
project(SteganographyProject)

set(CMAKE_CXX_STANDARD 17)

# Явное указание файлов
set(SOURCES
        main.cpp
        functions.cpp
)

# Подключаем директории
include_directories(.)

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Для Linux/macOS: математическая библиотека
if(UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif()