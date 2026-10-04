cmake_minimum_required(VERSION 3.20)
project(VisualizadorEstructuras VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- OPTIMIZACIÓN 1: Caché de compilación ---
# Si el usuario tiene ccache instalado, las compilaciones subsecuentes serán casi instantáneas
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()

# --- OPTIMIZACIÓN 2: Configuración de Release por defecto ---
# Si no se especifica el tipo de build, usa Release (más rápido al ejecutar, optimizado)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

include(FetchContent)

# --- OPTIMIZACIÓN 3: Apagar extras de SFML para compilar más rápido ---
set(SFML_BUILD_DOC_API OFF CACHE BOOL "Disable SFML docs" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "Disable SFML examples" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "Disable SFML tests" FORCE)

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
FetchContent_MakeAvailable(imgui) 

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