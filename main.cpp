#include <chrono>
#include <iostream>
#include <memory>
#include <string>

#include "ListaTareas.h"
#include "Tarea.h"
#include "TareaConFecha.h"
#include "TareaPrioritaria.h"
#include "Utilidades.h"

using namespace std;

// Agrega cualquier tipo de tarea a la lista y avisa si era repetida.
static void agregar(ListaTareas& lista, unique_ptr<Tarea> tarea) {
    if (lista.agregarTarea(move(tarea))) {
        cout << "Tarea agregada.\n";
    }
    else {
        cout << "Ya existe una tarea igual. No se agrego.\n";
    }
}

int main() {
    ListaTareas lista;
    bool salir = false;

    while (!salir) {
        mostrarMenu();

        int option = leerEntero("Seleccione una opcion: ", 0, 7);
        cout << endl;

        switch (option) {
        case 1: {
            string desc = leerLineaNoVacia("Descripcion de la tarea: ");
            agregar(lista, make_unique<Tarea>(desc, chrono::system_clock::now()));
            break;
        }

        case 2: {
            string desc = leerLineaNoVacia("Descripcion de la tarea: ");
            auto limite = leerFecha("Fecha limite (YYYY/MM/DD o YYYY/MM/DD HH:MM:SS): ");
            agregar(lista, make_unique<TareaConFecha>(desc, chrono::system_clock::now(), limite));
            break;
        }

        case 3: {
            string desc = leerLineaNoVacia("Descripcion de la tarea: ");
            cout << "Urgencia:\n";
            cout << "1. Baja\n";
            cout << "2. Media\n";
            cout << "3. Alta\n";
            int urg = leerEntero("Seleccione la urgencia: ", 1, 3);
            agregar(lista, make_unique<TareaPrioritaria>(desc, chrono::system_clock::now(),
                                                         static_cast<Urgencia>(urg)));
            break;
        }

        case 4: {
            lista.listarTareas();
            break;
        }

        case 5: {
            lista.ordenar();
            cout << "Tareas ordenadas (prioridad alta y fechas cercanas primero):\n";
            lista.listarTareas();
            break;
        }

        case 6: {
            if (lista.contar() == 0) {
                cout << "No hay tareas para cambiar de estado.\n";
                break;
            }

            lista.listarTareas();

            int idx = leerEntero("Numero de tarea a modificar: ", 1,
                                 static_cast<int>(lista.contar()));

            cout << "Estados:\n";
            cout << "1. Pendiente\n";
            cout << "2. En progreso\n";
            cout << "3. Completada\n";

            int est = leerEntero("Seleccione nuevo estado: ", 1, 3);

            if (lista.cambiarEstado(idx, static_cast<Estado>(est))) {
                cout << "Estado actualizado.\n";
            }
            else {
                cout << "Error al actualizar estado.\n";
            }
            break;
        }

        case 7: {
            if (lista.contar() == 0) {
                cout << "No hay tareas para eliminar.\n";
                break;
            }

            lista.listarTareas();

            int idx = leerEntero("Numero de tarea a eliminar: ", 1,
                                 static_cast<int>(lista.contar()));

            if (lista.eliminarTarea(idx)) {
                cout << "Tarea eliminada.\n";
            }
            else {
                cout << "Error al eliminar tarea.\n";
            }
            break;
        }

        case 0:
            salir = true;
            break;

        default:
            break;
        }

        cout << endl;
    }

    cout << "Apagando...\n";
    return 0;
}
