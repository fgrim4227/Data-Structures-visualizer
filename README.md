# Visualizador de Estructuras de datos (por los momentos solamente árboles)

Este proyecto es una herramienta educativa e interactiva escrita en **C++20** que permite visualizar el comportamiento de múltiples estructuras de datos en tiempo real. Utiliza la librería **SFML 3** para el renderizado gráfico, **ImGui** para la interfaz, y una arquitectura limpia orientada a la manipulación directa de memoria.

## Características
- **Múltiples Estructuras**: Soporte para Árbol Binario de Búsqueda (BST), Árbol AVL (Auto-balanceado), Treap y Listas Doblemente Enlazadas.
- **Visualización Dinámica**: Navegación en un lienzo 2D infinito (click derecho para arrastrar, rueda del ratón para hacer zoom).
- **Interacción en Tiempo Real**: Inserción y eliminación de nodos a través de una interfaz gráfica lateral interactiva.
- **Compilación Automatizada**: Configurado con CMake y FetchContent para gestionar SFML e ImGui automáticamente sin instalaciones manuales pesadas.

## Requisitos Técnicos
- **Compilador**: Compatible con C++20 (GCC 11+, Clang 13+, MSVC 19.30+).
- **Sistema de Construcción**: CMake 3.20 o superior.
- **Librerías**: SFML 3.0.0 e ImGui (descargadas automáticamente por el script de CMake).
- **Dependencias del Sistema (Linux)**:
  Para compilar SFML 3 en distribuciones basadas en Ubuntu/Debian, es necesario instalar las librerías de desarrollo de gráficos y ventanas:
  ```bash
  sudo apt update && sudo apt install -y libx11-dev libxrandr-dev libxcursor-dev libxi-dev libudev-dev libgl1-mesa-dev libfreetype6-dev    
    ```

## Instalación y Uso

### 1. Clonar el repositorio

```bash
git clone https://github.com/fgrim4227/Data-Structures-visualizer.git
cd Data-Structures-visualizer

```

### 2. Compilar el proyecto

Al utilizar `FetchContent`, la primera compilación descargará y construirá SFML e ImGui desde cero. Para que este proceso sea rápido, **se recomienda usar Ninja**.

#### Opción A: Compilación Ultra Rápida (Recomendada)

Para reducir drásticamente el tiempo de compilación (de minutos a segundos), instala el sistema de construcción `Ninja` y `ccache`.

**Instalación de herramientas (Ubuntu/Debian):**

```bash
sudo apt install ninja-build ccache

```

**Compilación:**

```bash
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .. && ninja

```

#### Opción B: Compilación Estándar

Si no deseas instalar herramientas adicionales, puedes usar el método tradicional con `make`, pero asegúrate de utilizar el flag `-j` seguido del número de núcleos de tu procesador para paralelizar el trabajo.

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build . -j4

```

### 3. Ejecutar

Una vez compilado, puedes iniciar el visualizador desde la carpeta `build`:

```bash
./VisualizadorEstructuras

```