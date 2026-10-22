#ifndef LISTA_TAREAS_H
#define LISTA_TAREAS_H

#include <memory>
#include <vector>

#include "Tarea.h"

// Coleccion polimorfica: guarda punteros a la clase base Tarea,
// pero cada elemento puede ser Tarea, TareaConFecha o TareaPrioritaria.
// unique_ptr libera la memoria automaticamente al eliminar.
class ListaTareas {
private:
    std::vector<std::unique_ptr<Tarea>> tareas_;

public:
    // Devuelve false si ya existe una tarea igual (usa operator==).
    bool agregarTarea(std::unique_ptr<Tarea> tarea);

    size_t contar() const;

    void listarTareas() const;

    // Ordena usando operator< (prioridad y despues fecha).
    void ordenar();

    bool cambiarEstado(size_t indice, Estado nuevoEstado);
    bool eliminarTarea(size_t indice);
};

#endif
