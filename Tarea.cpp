#include "Tarea.h"

#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

Tarea::Tarea(const string& description,
             const chrono::system_clock::time_point& creationDate)
    : description_(description),
      estado_(Estado::Pendiente),
      creationDate_(creationDate)
{
}

string Tarea::getDescription() const {
    return description_;
}

Estado Tarea::getEstado() const {
    return estado_;
}

string Tarea::getCreationDate() const {
    return formatearFecha(creationDate_);
}

chrono::system_clock::time_point Tarea::getCreationTimePoint() const {
    return creationDate_;
}

void Tarea::setDescription(const string& description) {
    description_ = description;
}

void Tarea::setEstado(Estado estado) {
    estado_ = estado;
}

string Tarea::tipo() const {
    return "Normal";
}

void Tarea::mostrar(ostream& os) const {
    os << "[" << estadoToString(estado_) << "] "
       << description_
       << " (Creada: " << getCreationDate() << ")";
}

int Tarea::peso() const {
    return 0;  // una tarea normal no tiene prioridad especial
}

chrono::system_clock::time_point Tarea::fechaReferencia() const {
    return creationDate_;
}

bool Tarea::operator<(const Tarea& otra) const {
    // peso() y fechaReferencia() son virtuales, asi que aqui
    // se usa la version de la clase real de cada tarea.
    if (peso() != otra.peso()) {
        return peso() > otra.peso();
    }
    return fechaReferencia() < otra.fechaReferencia();
}

static string enMinusculas(string texto) {
    for (char& c : texto) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return texto;
}

bool Tarea::operator==(const Tarea& otra) const {
    return tipo() == otra.tipo() &&
           enMinusculas(description_) == enMinusculas(otra.description_);
}

string Tarea::estadoToString(Estado estado) {
    switch (estado) {
    case Estado::Pendiente:
        return "Pendiente";
    case Estado::EnProgreso:
        return "En progreso";
    case Estado::Completada:
        return "Completada";
    default:
        return "Desconocido";
    }
}

string Tarea::formatearFecha(const chrono::system_clock::time_point& tp) {
    time_t tt = chrono::system_clock::to_time_t(tp);
    tm tmStruct{};

#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tmStruct, &tt);
#else
    localtime_r(&tt, &tmStruct);
#endif

    ostringstream oss;
    oss << put_time(&tmStruct, "%Y/%m/%d %H:%M");
    return oss.str();
}

ostream& operator<<(ostream& os, const Tarea& tarea) {
    tarea.mostrar(os);
    return os;
}
