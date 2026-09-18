#include "Tarea.h"
#include <ctime>
#include <sstream>
#include <iomanip>

std::string estadoATexto(Estado estado) {
    switch (estado) {
    case Estado::PENDIENTE:    return "Pendiente";
    case Estado::EN_PROGRESO:  return "En progreso";
    case Estado::COMPLETADA:   return "Completada";
    }
    return "Desconocido";
}

Estado textoAEstado(int opcion) {
    switch (opcion) {
    case 1: return Estado::PENDIENTE;
    case 2: return Estado::EN_PROGRESO;
    case 3: return Estado::COMPLETADA;
    default: return Estado::PENDIENTE;
    }
}

// Genera la fecha/hora actual como texto, usada al crear una tarea.
static std::string fechaActual() {
    std::time_t t = std::time(nullptr);
    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return oss.str();
}

Tarea::Tarea(int id, const std::string& descripcion)
    : id(id), descripcion(descripcion), estado(Estado::PENDIENTE), fechaCreacion(fechaActual()) {}

int Tarea::getId() const {
    return id;
}

std::string Tarea::getDescripcion() const {
    return descripcion;
}

Estado Tarea::getEstado() const {
    return estado;
}

std::string Tarea::getFechaCreacion() const {
    return fechaCreacion;
}

bool Tarea::setDescripcion(const std::string& nuevaDescripcion) {
    if (nuevaDescripcion.empty()) {
        return false;
    }
    descripcion = nuevaDescripcion;
    return true;
}

void Tarea::setEstado(Estado nuevoEstado) {
    estado = nuevoEstado;
}

std::string Tarea::toString() const {
    std::ostringstream oss;
    oss << "[" << id << "] "
        << descripcion
        << " | Estado: " << estadoATexto(estado)
        << " | Creada: " << fechaCreacion;
    return oss.str();
}
