#include "ListaTareas.h"

#include <algorithm>
#include <string>

#include "Excepciones.h"

using namespace std;

void ListaTareas::validarIndice(size_t indice) const {
    if (indice >= tareas_.size()) {
        throw ErrorValidacion("La tarea numero " + to_string(indice + 1) + " no existe.");
    }
}

bool ListaTareas::existeIgual(const Tarea& tarea, size_t ignorar) const {
    for (size_t i = 0; i < tareas_.size(); ++i) {
        if (i != ignorar && *tareas_[i] == tarea) {   // operator== sobrecargado
            return true;
        }
    }
    return false;
}

void ListaTareas::agregarTarea(unique_ptr<Tarea> tarea) {
    if (!tarea) {
        throw ErrorValidacion("No se recibio ninguna tarea para agregar.");
    }
    if (existeIgual(*tarea, tareas_.size())) {
        throw ErrorValidacion("Ya existe una tarea igual: \"" + tarea->getDescription() + "\".");
    }
    tareas_.push_back(move(tarea));
}

void ListaTareas::editarTarea(size_t indice, unique_ptr<Tarea> nueva) {
    validarIndice(indice);
    if (!nueva) {
        throw ErrorValidacion("No se recibio ninguna tarea para editar.");
    }
    if (existeIgual(*nueva, indice)) {
        throw ErrorValidacion("Ya existe otra tarea igual: \"" + nueva->getDescription() + "\".");
    }
    nueva->setId(tareas_[indice]->getId());  // sigue siendo la misma fila en la BD
    tareas_[indice] = move(nueva);
}

size_t ListaTareas::contar() const {
    return tareas_.size();
}

bool ListaTareas::vacia() const {
    return tareas_.empty();
}

const Tarea& ListaTareas::obtener(size_t indice) const {
    validarIndice(indice);
    return *tareas_[indice];
}

void ListaTareas::ordenar() {
    stable_sort(tareas_.begin(), tareas_.end(),
        [](const unique_ptr<Tarea>& a, const unique_ptr<Tarea>& b) {
            return *a < *b;   // operator< sobrecargado
        });
}

void ListaTareas::cambiarEstado(size_t indice, Estado nuevoEstado) {
    validarIndice(indice);
    tareas_[indice]->setEstado(nuevoEstado);
}

void ListaTareas::asignarId(size_t indice, int id) {
    validarIndice(indice);
    tareas_[indice]->setId(id);
}

void ListaTareas::eliminarTarea(size_t indice) {
    validarIndice(indice);
    tareas_.erase(tareas_.begin() + static_cast<ptrdiff_t>(indice));
}

void ListaTareas::limpiar() {
    tareas_.clear();
}
