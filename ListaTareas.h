#ifndef LISTA_TAREAS_H
#define LISTA_TAREAS_H

#include <iostream>
#include <locale>
#include <vector>

#include "Tarea.h"

using namespace std;

class ListaTareas {
private:
    vector<Tarea> tareas_;

public:
    void agregarTarea(
        const string& description,
        const chrono::system_clock::time_point& creationDate)
    {
        tareas_.emplace_back(description, creationDate);
    }

    size_t contar() const {
        return tareas_.size();
    }

    void listarTareas() const {
        if (tareas_.empty()) {
            cout << "No hay tareas.\n";
            return;
        }

        for (size_t i = 0; i < tareas_.size(); ++i) {
            const Tarea& tarea = tareas_[i];

            cout << (i + 1)
                << ". ["
                << Tarea::estadoToString(tarea.getEstado())
                << "] "
                << tarea.getDescription()
                << " (Creada: "
                << tarea.getCreationDate()
                << ")\n";
        }
    }

    bool cambiarEstado(size_t indice, Estado nuevoEstado) {
        if (indice >= 1 && indice <= tareas_.size()) {
            tareas_[indice - 1].setEstado(nuevoEstado);
            return true;
        }

        return false;
    }

    bool eliminarTarea(size_t indice) {
        if (indice >= 1 && indice <= tareas_.size()) {
            tareas_.erase(tareas_.begin() + (indice - 1));
            return true;
        }

        return false;
    }
};

#endif