#include "Tarea.h"

#include "Excepciones.h"

#include <cctype>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

Tarea::Tarea(const string& description,
             const chrono::system_clock::time_point& creationDate)
    : id_(0),
      estado_(Estado::Pendiente),
      creationDate_(creationDate)
{
    setDescription(description);  // valida que no este vacia
}

int Tarea::getId() const {
    return id_;
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

void Tarea::setId(int id) {
    id_ = id;
}

void Tarea::setDescription(const string& description) {
    bool soloEspacios = true;
    for (char c : description) {
        if (!isspace(static_cast<unsigned char>(c))) {
            soloEspacios = false;
            break;
        }
    }
    if (soloEspacios) {
        throw ErrorValidacion("La descripcion de la tarea no puede estar vacia.");
    }
    description_ = description;
}

void Tarea::setEstado(Estado estado) {
    estado_ = estado;
}

string Tarea::tipo() const {
    return "Normal";
}

string Tarea::detalle() const {
    return "";  // una tarea normal no tiene datos extra
}

string Tarea::datoExtra() const {
    return "";
}

void Tarea::mostrar(ostream& os) const {
    os << "[" << estadoToString(estado_) << "] "
       << description_
       << " (Creada: " << getCreationDate() << ")";
}

string Tarea::texto() const {
    ostringstream oss;
    mostrar(oss);  // virtual: usa la version de la clase real
    return oss.str();
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

long long Tarea::aSegundos(const chrono::system_clock::time_point& tp) {
    return chrono::duration_cast<chrono::seconds>(tp.time_since_epoch()).count();
}

chrono::system_clock::time_point Tarea::desdeSegundos(long long segundos) {
    return chrono::system_clock::time_point(chrono::seconds(segundos));
}

ostream& operator<<(ostream& os, const Tarea& tarea) {
    tarea.mostrar(os);
    return os;
}
