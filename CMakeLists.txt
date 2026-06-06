# Минимальная версия CMake
cmake_minimum_required(VERSION 3.10)

# Название проекта
project(N_bodies VERSION 1.0.0 LANGUAGES CXX)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Поиск пакета SFML версии 2.6
find_package(SFML 2.6 REQUIRED COMPONENTS system window graphics audio network)

# Проверка, найден ли SFML
if(SFML_FOUND)
    message(STATUS "SFML found successfully")
    message(STATUS "SFML Include Directories: ${SFML_INCLUDE_DIR}")
    message(STATUS "SFML Libraries: ${SFML_LIBRARIES}")
else()
    message(FATAL_ERROR "SFML 2.6 not found. Please install SFML 2.6")
endif()

# Сборка всех исходных файлов из директории ./src
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "headers/*.h" "headers/*.hpp")

# Проверка, найдены ли исходные файлы
if(NOT SOURCES)
    message(FATAL_ERROR "No source files found in ./src directory")
endif()

# Создание исполняемого файла
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Установка директорий для заголовочных файлов
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${SFML_INCLUDE_DIR}
)

# Линковка с библиотеками SFML
target_link_libraries(${PROJECT_NAME} 
    ${SFML_LIBRARIES}
    sfml-system
    sfml-window
    sfml-graphics
    sfml-audio
    sfml-network
)

# Настройка для Windows (копирование DLL-файлов)
if(WIN32)
    # Путь к DLL-файлам SFML (пусть пользователь укажет или автоматически найдет)
    get_target_property(SFML_LOCATION sfml-system IMPORTED_LOCATION_RELEASE)
    if(NOT SFML_LOCATION)
        get_target_property(SFML_LOCATION sfml-system IMPORTED_LOCATION)
    endif()
    
    if(SFML_LOCATION)
        get_filename_component(SFML_BIN_DIR ${SFML_LOCATION} DIRECTORY)
        
        # Копирование DLL в директорию с исполняемым файлом
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${SFML_BIN_DIR}/sfml-system-2.dll"
                "${SFML_BIN_DIR}/sfml-window-2.dll"
                "${SFML_BIN_DIR}/sfml-graphics-2.dll"
                "${SFML_BIN_DIR}/sfml-audio-2.dll"
                "${SFML_BIN_DIR}/sfml-network-2.dll"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying SFML DLLs to output directory"
        )
    endif()
endif()

# Вывод информации о сборке
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Sources found: ${SOURCES}")
message(STATUS "Headers found: ${HEADERS}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
