#include <iostream>
#include <limits>
#include <string>
#include "Tarea.h"
#include "ListaTareas.h"

// Lee un entero validado desde consola. Repite hasta recibir un número entero
// válido; evita que el programa truene si el usuario escribe texto no numérico.
int leerEntero(const std::string& mensaje) {
    int valor;
    while (true) {
        std::cout << mensaje;
        std::cin >> valor;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Entrada inválida. Por favor ingresa un número entero.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return valor;
    }
}

// Lee una línea de texto no vacía desde consola.
std::string leerLineaNoVacia(const std::string& mensaje) {
    std::string valor;
    while (true) {
        std::cout << mensaje;
        std::getline(std::cin, valor);
        if (valor.empty()) {
            std::cout << "La descripción no puede estar vacía.\n";
            continue;
        }
        return valor;
    }
}

// Pide al usuario un id y valida que exista en la lista antes de continuar.
// Devuelve -1 si el usuario cancela (id 0).
int pedirIdValido(const ListaTareas& lista, const std::string& accion) {
    if (lista.cantidad() == 0) {
        std::cout << "No hay tareas registradas.\n";
        return -1;
    }
    lista.listarTareas();
    int id = leerEntero("ID de la tarea a " + accion + " (0 para cancelar): ");
    if (id == 0) {
        return -1;
    }
    if (!lista.existeId(id)) {
        std::cout << "No existe una tarea con ese ID.\n";
        return -1;
    }
    return id;
}

void mostrarMenu() {
    std::cout << "\n===== Lista de Tareas =====\n"
        << "1. Agregar tarea\n"
        << "2. Listar tareas\n"
        << "3. Cambiar estado de una tarea\n"
        << "4. Eliminar tarea\n"
        << "5. Salir\n"
        << "============================\n";
}

Estado pedirEstado() {
    std::cout << "Selecciona el nuevo estado:\n"
        << "  1. Pendiente\n"
        << "  2. En progreso\n"
        << "  3. Completada\n";
    int opcion;
    while (true) {
        opcion = leerEntero("Opción: ");
        if (opcion >= 1 && opcion <= 3) {
            return textoAEstado(opcion);
        }
        std::cout << "Opción inválida, elige 1, 2 o 3.\n";
    }
}

int main() {
    ListaTareas lista;
    bool salir = false;

    while (!salir) {
        mostrarMenu();
        int opcion = leerEntero("Elige una opción: ");

        switch (opcion) {
        case 1: {
            std::string descripcion = leerLineaNoVacia("Descripción de la tarea: ");
            int id = lista.agregarTarea(descripcion);
            std::cout << "Tarea agregada con ID " << id << ".\n";
            break;
        }
        case 2: {
            std::cout << "\n-- Tareas --\n";
            lista.listarTareas();
            break;
        }
        case 3: {
            int id = pedirIdValido(lista, "modificar");
            if (id != -1) {
                Estado nuevo = pedirEstado();
                lista.cambiarEstado(id, nuevo);
                std::cout << "Estado actualizado.\n";
            }
            break;
        }
        case 4: {
            int id = pedirIdValido(lista, "eliminar");
            if (id != -1) {
                lista.eliminarTarea(id);
                std::cout << "Tarea eliminada.\n";
            }
            break;
        }
        case 5: {
            salir = true;
            std::cout << "¡Hasta luego!\n";
            break;
        }
        default: {
            std::cout << "Opción inválida, intenta de nuevo.\n";
            break;
        }
        }
    }

    return 0;
}
