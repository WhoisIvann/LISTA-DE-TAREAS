// Version de consola (Fases 1 y 2). La version final con ventanas esta en main.cpp.
// Toda la entrada/salida de consola vive aqui y en Utilidades.cpp;
// las clases del modelo ya no usan cin ni cout.

#include <chrono>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "Excepciones.h"
#include "ListaTareas.h"
#include "Tarea.h"
#include "TareaConFecha.h"
#include "TareaPrioritaria.h"
#include "Utilidades.h"

using namespace std;

static void listar(const ListaTareas& lista) {
    if (lista.vacia()) {
        cout << "No hay tareas.\n";
        return;
    }

    for (size_t i = 0; i < lista.contar(); ++i) {
        // Mismo codigo para todos los tipos: cada objeto decide
        // como mostrarse gracias a tipo() y mostrar() virtuales.
        const Tarea& t = lista.obtener(i);
        cout << setw(2) << (i + 1) << ". "
             << left << setw(14) << ("<" + t.tipo() + ">") << right
             << t << "\n";
    }
}

// Pide al usuario el numero de una tarea y lo regresa como indice (desde 0).
static size_t elegirTarea(const ListaTareas& lista, const string& prompt) {
    listar(lista);
    int numero = leerEntero(prompt, 1, static_cast<int>(lista.contar()));
    return static_cast<size_t>(numero - 1);
}

int main() {
    ListaTareas lista;
    bool salir = false;

    while (!salir) {
        mostrarMenu();

        int option = leerEntero("Seleccione una opcion: ", 0, 7);
        cout << endl;

        try {
            switch (option) {
            case 1: {
                string desc = leerLineaNoVacia("Descripcion de la tarea: ");
                lista.agregarTarea(make_unique<Tarea>(desc, chrono::system_clock::now()));
                cout << "Tarea agregada.\n";
                break;
            }

            case 2: {
                string desc = leerLineaNoVacia("Descripcion de la tarea: ");
                auto limite = leerFecha("Fecha limite (YYYY/MM/DD o YYYY/MM/DD HH:MM:SS): ");
                lista.agregarTarea(make_unique<TareaConFecha>(desc, chrono::system_clock::now(), limite));
                cout << "Tarea agregada.\n";
                break;
            }

            case 3: {
                string desc = leerLineaNoVacia("Descripcion de la tarea: ");
                cout << "Urgencia:\n";
                cout << "1. Baja\n";
                cout << "2. Media\n";
                cout << "3. Alta\n";
                int urg = leerEntero("Seleccione la urgencia: ", 1, 3);
                lista.agregarTarea(make_unique<TareaPrioritaria>(desc, chrono::system_clock::now(),
                                                                 static_cast<Urgencia>(urg)));
                cout << "Tarea agregada.\n";
                break;
            }

            case 4:
                listar(lista);
                break;

            case 5:
                lista.ordenar();
                cout << "Tareas ordenadas (prioridad alta y fechas cercanas primero):\n";
                listar(lista);
                break;

            case 6: {
                if (lista.vacia()) {
                    cout << "No hay tareas para cambiar de estado.\n";
                    break;
                }

                size_t idx = elegirTarea(lista, "Numero de tarea a modificar: ");

                cout << "Estados:\n";
                cout << "1. Pendiente\n";
                cout << "2. En progreso\n";
                cout << "3. Completada\n";

                int est = leerEntero("Seleccione nuevo estado: ", 1, 3);
                lista.cambiarEstado(idx, static_cast<Estado>(est));
                cout << "Estado actualizado.\n";
                break;
            }

            case 7: {
                if (lista.vacia()) {
                    cout << "No hay tareas para eliminar.\n";
                    break;
                }

                size_t idx = elegirTarea(lista, "Numero de tarea a eliminar: ");
                lista.eliminarTarea(idx);
                cout << "Tarea eliminada.\n";
                break;
            }

            case 0:
                salir = true;
                break;

            default:
                break;
            }
        }
        catch (const ErrorTareas& e) {
            cout << e.titulo() << ": " << e.what() << "\n";
        }

        cout << endl;
    }

    cout << "Apagando...\n";
    return 0;
}
