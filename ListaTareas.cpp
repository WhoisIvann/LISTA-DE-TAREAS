#include "ListaTareas.h"
#include <iostream>

ListaTareas::ListaTareas() : siguienteId(1) {}

int ListaTareas::buscarIndicePorId(int id) const {
    for (size_t i = 0; i < tareas.size(); ++i) {
        if (tareas[i].getId() == id) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

int ListaTareas::agregarTarea(const std::string& descripcion) {
    if (descripcion.empty()) {
        return -1;
    }
    Tarea nueva(siguienteId, descripcion);
    tareas.push_back(nueva);
    return siguienteId++;
}

void ListaTareas::listarTareas() const {
    if (tareas.empty()) {
        std::cout << "No hay tareas registradas.\n";
        return;
    }
    for (const Tarea& t : tareas) {
        std::cout << t.toString() << "\n";
    }
}

bool ListaTareas::cambiarEstado(int id, Estado nuevoEstado) {
    int idx = buscarIndicePorId(id);
    if (idx == -1) {
        return false;
    }
    tareas[idx].setEstado(nuevoEstado);
    return true;
}

bool ListaTareas::eliminarTarea(int id) {
    int idx = buscarIndicePorId(id);
    if (idx == -1) {
        return false;
    }
    tareas.erase(tareas.begin() + idx);
    return true;
}

bool ListaTareas::existeId(int id) const {
    return buscarIndicePorId(id) != -1;
}

size_t ListaTareas::cantidad() const {
    return tareas.size();
}
