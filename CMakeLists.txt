cmake_minimum_required(VERSION 3.10)
project(SteganographyProject)

set(CMAKE_CXX_STANDARD 17)

# Основная программа
add_executable(${PROJECT_NAME}
        main.cpp
        functions.cpp
)

# Для Linux/macOS: математическая библиотека
if(UNIX)
    target_link_libraries(${PROJECT_NAME} m)
endif()

# Тесты
add_executable(tests
        test.cpp
        functions.cpp
)

# Для тестов тоже нужны математические библиотеки
if(UNIX)
    target_link_libraries(tests m)
endif()

# Подключаем директории для всех целей
include_directories(.)