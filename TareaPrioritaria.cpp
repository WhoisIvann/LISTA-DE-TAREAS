#include "TareaPrioritaria.h"

using namespace std;

TareaPrioritaria::TareaPrioritaria(const string& description,
                                   const chrono::system_clock::time_point& creationDate,
                                   Urgencia urgencia)
    : Tarea(description, creationDate),
      urgencia_(urgencia)
{
}

Urgencia TareaPrioritaria::getUrgencia() const {
    return urgencia_;
}

void TareaPrioritaria::setUrgencia(Urgencia urgencia) {
    urgencia_ = urgencia;
}

string TareaPrioritaria::urgenciaToString(Urgencia urgencia) {
    switch (urgencia) {
    case Urgencia::Baja:
        return "Baja";
    case Urgencia::Media:
        return "Media";
    case Urgencia::Alta:
        return "Alta";
    default:
        return "Desconocida";
    }
}

string TareaPrioritaria::tipo() const {
    return "Prioritaria";
}

void TareaPrioritaria::mostrar(ostream& os) const {
    Tarea::mostrar(os);
    os << " | Urgencia: " << urgenciaToString(urgencia_);
    if (urgencia_ == Urgencia::Alta) {
        os << " (!!!)";
    }
}

int TareaPrioritaria::peso() const {
    // Baja = 1, Media = 2, Alta = 3
    return static_cast<int>(urgencia_);
}
