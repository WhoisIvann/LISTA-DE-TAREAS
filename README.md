# To-Do List en C++ — Fase 1

Aplicación de consola en C++ para administrar una lista de tareas **en memoria**, usando clases y objetos. Corresponde a la Fase 1 del proyecto del semestre (fundamentos con POO); las fases 2 y 3 agregan herencia/polimorfismo y persistencia real en base de datos, respectivamente.

## Qué hace

Un menú de consola que permite:

- Agregar una tarea (descripción).
- Listar todas las tareas registradas.
- Cambiar el estado de una tarea (Pendiente / En progreso / Completada).
- Eliminar una tarea.

Todas las entradas se validan: un texto donde se espera un número, un ID que no existe o una descripción vacía no truenan el programa, solo piden el dato de nuevo.

## Estructura del código

| Archivo | Contenido |
|---|---|
| `Tarea.h` / `Tarea.cpp` | Clase `Tarea`: atributos privados (`id`, `descripcion`, `estado`, `fechaCreacion`), getters, setters con validación (`setDescripcion` rechaza cadenas vacías) y `toString()` para imprimirla. |
| `ListaTareas.h` / `ListaTareas.cpp` | Clase `ListaTareas`: administra un `std::vector<Tarea>` y expone `agregarTarea`, `listarTareas`, `cambiarEstado`, `eliminarTarea`, `existeId`, `cantidad`. Nada fuera de la clase toca el vector directamente. |
| `main.cpp` | Menú de consola (ciclo de opciones) y funciones auxiliares de entrada (`leerEntero`, `leerLineaNoVacia`, `pedirIdValido`, `pedirEstado`) que hacen la validación. |

## Compilar y ejecutar

```bash
g++ -std=c++17 -Wall -Wextra -o todo main.cpp Tarea.cpp ListaTareas.cpp
./todo
```

## Cómo trabajamos como equipo

- **Kanban**: tablero con columnas Por hacer / En proceso / Terminado (GitHub Projects), una tarjeta por clase o función.
- **Revisiones semanales**: reunión breve tipo sprint review — qué se movió, qué se atoró, qué sigue.
- **Commits**: repartidos entre integrantes y a lo largo de varias semanas, no todo de golpe al final.
- **Demo**: el día del examen práctico, cualquier integrante debe poder explicar y modificar el código en vivo.

## Relación con los criterios de evaluación

- **Funcionalidad del menú**: las 4 operaciones funcionan de principio a fin, probadas con entradas inválidas (texto en vez de número, ID inexistente, descripción vacía) sin que el programa se caiga.
- **Diseño orientado a objetos**: atributos privados con getters/setters que validan; `ListaTareas` administra objetos `Tarea` reales (no listas sueltas de strings ni arreglos primitivos), con responsabilidad separada de `main.cpp` (que solo maneja el menú).
- **Demo y dominio del código**: la estructura en 3 archivos (Tarea / ListaTareas / main) facilita que cada integrante pueda explicar la parte que le tocó.
- **GitHub y trabajo en equipo**: se espera evidencia de commits repartidos entre integrantes a lo largo de varias semanas, con mensajes claros.
- **Kanban y revisiones semanales**: tablero actualizado con evidencia de al menos 3 revisiones semanales documentadas.

## Posibles extensiones antes de entregar

- Documentar las revisiones semanales (captura del tablero o nota corta) como evidencia.
- Si `main.cpp` crece mucho, separar las funciones de entrada/validación en su propio archivo de utilidades.
