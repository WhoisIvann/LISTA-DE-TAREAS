# Lista de tareas

Aplicación de consola en C++ para administrar tareas. Proyecto de POO en 3 fases.

## Fase 2: herencia, polimorfismo y sobrecarga

No todas las tareas son iguales. Ahora hay tres tipos:

| Clase | Qué agrega | Cómo se muestra |
|---|---|---|
| `Tarea` (base) | descripción, estado, fecha de creación | `[Pendiente] Lavar los trastes (Creada: ...)` |
| `TareaConFecha` | fecha límite; detecta si está vencida | `... \| Limite: 2026/10/30 23:59` y `<-- VENCIDA` |
| `TareaPrioritaria` | urgencia Baja / Media / Alta | `... \| Urgencia: Alta (!!!)` |

- **Herencia:** `TareaConFecha` y `TareaPrioritaria` heredan de `Tarea`.
- **Polimorfismo:** `Tarea` tiene métodos `virtual` (`tipo()`, `mostrar()`, `peso()`, `fechaReferencia()`) y destructor virtual. Cada derivada los sobrescribe con `override`.
- **Colección polimórfica:** `ListaTareas` guarda `std::vector<std::unique_ptr<Tarea>>`, así que en la misma lista conviven los tres tipos y se listan y ordenan con el mismo código.
- **Sobrecarga de operadores:**
  - `operator<` compara por prioridad (peso) y, si empatan, por fecha. Se usa para ordenar la lista.
  - `operator==` detecta tareas repetidas (mismo tipo y misma descripción, sin importar mayúsculas).
  - `operator<<` permite hacer `cout << tarea` y llama a `mostrar()` de la clase real.

### Reglas de orden (opción 5 del menú)

| Tipo de tarea | Peso |
|---|---|
| Con fecha **vencida** / Prioritaria **Alta** | 3 |
| Prioritaria Media | 2 |
| Prioritaria Baja / Con fecha no vencida | 1 |
| Normal | 0 |

Mayor peso va primero; si empatan, va primero la fecha más cercana (fecha límite o de creación).

## Menú

1. Agregar tarea normal
2. Agregar tarea con fecha límite
3. Agregar tarea prioritaria
4. Listar tareas
5. Ordenar por prioridad y fecha
6. Cambiar estado de una tarea
7. Eliminar tarea
0. Salir

Las tareas se guardan en memoria y se pierden al cerrar el programa (la persistencia llega en la Fase 3).

## Archivos

- `Tarea.h` / `Tarea.cpp`: clase base y operadores.
- `TareaConFecha.h` / `TareaConFecha.cpp`: tarea con fecha límite.
- `TareaPrioritaria.h` / `TareaPrioritaria.cpp`: tarea con urgencia.
- `ListaTareas.h` / `ListaTareas.cpp`: colección polimórfica.
- `Utilidades.h` / `Utilidades.cpp`: lectura y validación de entradas, menú.
- `main.cpp`: menú principal.

## Cómo ejecutarlo

**Visual Studio:** abre `LISTA DE TAREAS.slnx`, compila y ejecuta (C++20).

**Terminal (Mac/Linux):**

```
g++ -std=c++20 -Wall -o todo main.cpp Tarea.cpp TareaConFecha.cpp TareaPrioritaria.cpp ListaTareas.cpp Utilidades.cpp
./todo
```

## Próximas mejoras (Fase 3)

- Guardar las tareas en base de datos y archivos.
- Manejo de errores con excepciones.
- Permitir editar la descripción de las tareas.
