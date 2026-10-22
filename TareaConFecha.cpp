#include "TareaConFecha.h"

using namespace std;

TareaConFecha::TareaConFecha(const string& description,
                             const chrono::system_clock::time_point& creationDate,
                             const chrono::system_clock::time_point& fechaLimite)
    : Tarea(description, creationDate),
      fechaLimite_(fechaLimite)
{
}

chrono::system_clock::time_point TareaConFecha::getFechaLimite() const {
    return fechaLimite_;
}

void TareaConFecha::setFechaLimite(const chrono::system_clock::time_point& fechaLimite) {
    fechaLimite_ = fechaLimite;
}

bool TareaConFecha::estaVencida() const {
    return getEstado() != Estado::Completada &&
           chrono::system_clock::now() > fechaLimite_;
}

string TareaConFecha::tipo() const {
    return "Con fecha";
}

void TareaConFecha::mostrar(ostream& os) const {
    Tarea::mostrar(os);  // primero lo comun de cualquier tarea
    os << " | Limite: " << formatearFecha(fechaLimite_);
    if (estaVencida()) {
        os << " <-- VENCIDA";
    }
}

int TareaConFecha::peso() const {
    // Una tarea vencida se trata como urgente (igual que prioridad Alta).
    return estaVencida() ? 3 : 1;
}

chrono::system_clock::time_point TareaConFecha::fechaReferencia() const {
    // Al ordenar, la que vence antes va primero.
    return fechaLimite_;
}
