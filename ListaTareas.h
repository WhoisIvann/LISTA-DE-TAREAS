#ifndef LISTA_TAREAS_H
#define LISTA_TAREAS_H

#include <memory>
#include <vector>

#include "Tarea.h"

// Coleccion polimorfica: guarda punteros a la clase base Tarea,
// pero cada elemento puede ser Tarea, TareaConFecha o TareaPrioritaria.
// unique_ptr libera la memoria automaticamente al eliminar.
//
// Esta clase NO lee ni escribe en consola ni crea ventanas: solo guarda
// los datos. Los errores se avisan lanzando ErrorValidacion (Excepciones.h).
// Los indices empiezan en 0.
class ListaTareas {
private:
    std::vector<std::unique_ptr<Tarea>> tareas_;

    // Lanza ErrorValidacion si el indice no existe.
    void validarIndice(size_t indice) const;

    // true si ya hay una tarea igual (operator==), sin contar la del indice ignorar.
    bool existeIgual(const Tarea& tarea, size_t ignorar) const;

public:
    // Lanza ErrorValidacion si la tarea es nula o ya existe una igual.
    void agregarTarea(std::unique_ptr<Tarea> tarea);

    // Reemplaza la tarea del indice por otra (puede ser de otro tipo).
    // Conserva el id de la base de datos de la tarea original.
    // Lanza ErrorValidacion si el indice no existe o quedaria repetida.
    void editarTarea(size_t indice, std::unique_ptr<Tarea> nueva);

    size_t contar() const;
    bool vacia() const;

    // Acceso a una tarea para mostrarla. Lanza ErrorValidacion si no existe.
    const Tarea& obtener(size_t indice) const;

    // Ordena usando operator< (prioridad y despues fecha).
    void ordenar();

    void cambiarEstado(size_t indice, Estado nuevoEstado);
    void eliminarTarea(size_t indice);

    // Borra todas las tareas (por ejemplo, antes de cargar de la base de datos).
    void limpiar();
};

#endif
