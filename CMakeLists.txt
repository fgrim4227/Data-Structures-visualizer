cmake_minimum_required(VERSION 3.20)
project(VisualizadorEstructuras VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# 1. Descargar SFML 3
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

# 2. Descargar ImGui base (v1.91.5 para compatibilidad con ImGui-SFML v3.0)
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.91.5 
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

# 3. Configuración de rutas para los "puentes"
set(IMGUI_DIR ${imgui_SOURCE_DIR} CACHE PATH "Path to ImGui")
set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "")
set(SFML_DIR ${sfml_BINARY_DIR} CACHE PATH "")

# 4. Descargar ImGui-SFML (El puente)
FetchContent_Declare(
    imgui-sfml
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
    GIT_TAG v3.0
)
FetchContent_MakeAvailable(imgui-sfml)

# 5. Configuración del Proyecto
include_directories(include ${imgui_SOURCE_DIR}) # Añadimos imgui al include path
file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} main.cpp ${SOURCES})

target_link_libraries(${PROJECT_NAME} 
    sfml-graphics 
    sfml-window 
    sfml-system 
    ImGui-SFML::ImGui-SFML
)

# Copiar assets (fuentes, etc)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})