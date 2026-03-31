cmake_minimum_required(VERSION 3.20)
project(VisualizadorEstructuras VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Traer SFML desde GitHub
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

# 2. Configurar directorios e incluir archivos
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
# 3. Crear el ejecutable (Solo una vez)
# Incluimos main.cpp y todos los archivos encontrados en src/
add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

# 4. Enlazar (Nota: FetchContent usa los nombres de target de SFML)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)