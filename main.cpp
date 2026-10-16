#include <clocale>
#include <iostream>
#include <string>

#include "ListaTareas.h"
#include "Utilidades.h"

using namespace std;

int main() {

    ListaTareas lista;
    bool salir = false;

    while (!salir) {
        mostrarMenu();

        int option = leerEntero("Seleccione una opcion: ", 0, 4);
        cout << endl;

        switch (option) {
        case 1: {
            string desc = leerLineaNoVacia("Descripcion de la tarea: ");
            auto fecha = leerFecha(
                "Fecha de creación (YYYY/MM/DD o YYYY/MM/DD HH:MM:SS): ");

            lista.agregarTarea(desc, fecha);
            cout << "Tarea agregada.\n";
            break;
        }

        case 2: {
            lista.listarTareas();
            break;
        }

        case 3: {
            if (lista.contar() == 0) {
                cout << "No hay tareas para cambiar de estado.\n";
                break;
            }

            lista.listarTareas();

            int idx = leerEntero(
                "Numero de tarea a modificar: ",
                1,
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

        case 4: {
            if (lista.contar() == 0) {
                cout << "No hay tareas para eliminar.\n";
                break;
            }

            lista.listarTareas();

            int idx = leerEntero(
                "Número de tarea a eliminar: ",
                1,
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