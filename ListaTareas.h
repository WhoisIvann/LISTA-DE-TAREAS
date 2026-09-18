#ifndef LISTATAREAS_H
#define LISTATAREAS_H

#include <vector>
#include "Tarea.h"

// Administra la colección de objetos Tarea: agregar, listar, cambiar estado
// y eliminar. Encapsula el vector de tareas: nadie fuera de la clase accede
// directamente al arreglo, solo a través de estos métodos.
class ListaTareas {
private:
    std::vector<Tarea> tareas;
    int siguienteId; // contador para asignar IDs únicos y crecientes

    // Busca el índice dentro de 'tareas' cuyo id coincide. Devuelve -1 si no existe.
    int buscarIndicePorId(int id) const;

public:
    ListaTareas();

    // Crea y agrega una tarea nueva. Devuelve el id asignado, o -1 si la
    // descripción no es válida (vacía).
    int agregarTarea(const std::string& descripcion);

    // Imprime todas las tareas en consola. Si la lista está vacía, lo indica.
    void listarTareas() const;

    // Cambia el estado de la tarea con el id dado. Devuelve false si el id no existe.
    bool cambiarEstado(int id, Estado nuevoEstado);

    // Elimina la tarea con el id dado. Devuelve false si el id no existe.
    bool eliminarTarea(int id);

    // Devuelve true si ya existe una tarea con ese id (útil para validar entradas).
    bool existeId(int id) const;

    // Cantidad de tareas actualmente en la lista.
    size_t cantidad() const;
};

#endif // LISTATAREAS_H

