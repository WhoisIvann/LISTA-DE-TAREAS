cmake_minimum_required(VERSION 3.16)

project(ListaDeTareas VERSION 3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt genera codigo extra (moc) para las clases con Q_OBJECT.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Los textos de la interfaz llevan acentos: compilar los fuentes como UTF-8.
if(MSVC)
    add_compile_options(/utf-8)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

# ---- Modelo: tareas sin consola ni ventanas (lo usan las dos versiones) ----
add_library(modelo STATIC
    Excepciones.h
    Tarea.h             Tarea.cpp
    TareaConFecha.h     TareaConFecha.cpp
    TareaPrioritaria.h  TareaPrioritaria.cpp
    ListaTareas.h       ListaTareas.cpp
)
target_include_directories(modelo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# ---- Aplicacion de escritorio (Fase 3) ----
# Agregar aqui los archivos nuevos: VentanaPrincipal, DialogoTarea, BaseDatos, ...
add_executable(ListaDeTareas
    main.cpp
    Aplicacion.h Aplicacion.cpp
)
target_link_libraries(ListaDeTareas PRIVATE modelo Qt6::Widgets Qt6::Sql)

# Sin ventana de consola negra detras de la app en Windows.
set_target_properties(ListaDeTareas PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

# ---- Version de consola (Fases 1 y 2), por si se quiere comparar ----
add_executable(ListaDeTareasConsola
    main_consola.cpp
    Utilidades.h Utilidades.cpp
)
target_link_libraries(ListaDeTareasConsola PRIVATE modelo)
