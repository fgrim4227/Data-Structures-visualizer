# Visualizador de Estructuras de datos (por los momentos solamente arboles)

Este proyecto es una herramienta educativa e interactiva escrita en **C++20** que permite visualizar el comportamiento de un **Árbol Binario de Búsqueda (BST)** en tiempo real. Utiliza la librería **SFML 3** para el renderizado gráfico y una lógica de punteros inteligentes para una gestión de memoria eficiente.

## Características
- **Visualización Dinámica**: Los nodos se posicionan automáticamente en un lienzo 2D.
- **Inserción e Interacción**: Agrega nodos presionando la tecla `A` y escribiendo el valor en la terminal.
- **Eliminación Lógica**: Implementación completa del borrado de nodos (hojas, un hijo, o dos hijos con sucesor inorder).
- **Arquitectura Moderna**: Uso de `std::shared_ptr` y `std::enable_shared_from_this` para evitar fugas de memoria y punteros colgantes.
- **Compilación Automatizada**: Configurado con CMake y FetchContent para gestionar SFML sin instalaciones manuales.

## Requisitos Técnicos
- **Compilador**: Compatible con C++20 (GCC 11+, Clang 13+, MSVC 19.30+).
- **Sistema de Construcción**: CMake 3.20 o superior.
- **Librerías**: SFML 3.0.0 (descargada automáticamente por el script de CMake).
- **Dependencias del Sistema (Linux)**:
    Para compilar SFML 3 en distribuciones basadas en Ubuntu/Debian, es necesario instalar las librerías de desarrollo de gráficos y ventanas:
    ```bash
    sudo apt update && sudo apt install -y libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev libfreetype6-dev    
    ```
## Instalación y Uso

1. **Clonar el repositorio**:
   ```bash
   git clone [https://github.com/tu-usuario/nombre-del-repo.git](https://github.com/tu-usuario/nombre-del-repo.git)
   cd nombre-del-repo

2. **Compilar el proyecto**:
    ```
    Bash
    mkdir build && cd build
    cmake ..
    cmake --build .
    ```
3. **Ejecutar**:
```
    Bash
    ./VisualizadorEstructuras
```