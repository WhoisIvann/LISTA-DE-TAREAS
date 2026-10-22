#include "ListaTareas.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

using namespace std;

bool ListaTareas::agregarTarea(unique_ptr<Tarea> tarea) {
    for (const auto& existente : tareas_) {
        if (*existente == *tarea) {   // operator== sobrecargado
            return false;
        }
    }
    tareas_.push_back(move(tarea));
    return true;
}

size_t ListaTareas::contar() const {
    return tareas_.size();
}

void ListaTareas::listarTareas() const {
    if (tareas_.empty()) {
        cout << "No hay tareas.\n";
        return;
    }

    for (size_t i = 0; i < tareas_.size(); ++i) {
        // Mismo codigo para todos los tipos: cada objeto decide
        // como mostrarse gracias a tipo() y mostrar() virtuales.
        cout << setw(2) << (i + 1) << ". "
             << left << setw(14) << ("<" + tareas_[i]->tipo() + ">") << right
             << *tareas_[i] << "\n";
    }
}

void ListaTareas::ordenar() {
    stable_sort(tareas_.begin(), tareas_.end(),
        [](const unique_ptr<Tarea>& a, const unique_ptr<Tarea>& b) {
            return *a < *b;   // operator< sobrecargado
        });
}

bool ListaTareas::cambiarEstado(size_t indice, Estado nuevoEstado) {
    if (indice >= 1 && indice <= tareas_.size()) {
        tareas_[indice - 1]->setEstado(nuevoEstado);
        return true;
    }
    return false;
}

bool ListaTareas::eliminarTarea(size_t indice) {
    if (indice >= 1 && indice <= tareas_.size()) {
        tareas_.erase(tareas_.begin() + (indice - 1));
        return true;
    }
    return false;
}
