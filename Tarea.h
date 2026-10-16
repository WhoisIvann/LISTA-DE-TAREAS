#ifndef TAREA_H
#define TAREA_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

using namespace std;

enum class Estado {
    Pendiente = 1,
    EnProgreso = 2,
    Completada = 3
};

class Tarea {
private:
    string description_;
    Estado estado_;
    chrono::system_clock::time_point creationDate_;

public:
    Tarea(const string& description, const chrono::system_clock::time_point& creationDate)
        : description_(description),
        estado_(Estado::Pendiente),
        creationDate_(creationDate)
    {
    }

    string getDescription() const {
        return description_;
    }

    Estado getEstado() const {
        return estado_;
    }

    string getCreationDate() const {
        time_t tt = chrono::system_clock::to_time_t(creationDate_);
        tm tmStruct;

#if defined(_WIN32) || defined(_WIN64)
        localtime_s(&tmStruct, &tt);
#else
        localtime_r(&tt, &tmStruct);
#endif

        ostringstream oss;
        oss << put_time(&tmStruct, "%Y/%m/%d %H:%M:%S");
        return oss.str();
    }

    void setDescription(const string& description) {
        description_ = description;
    }

    void setEstado(Estado estado) {
        estado_ = estado;
    }

    static string estadoToString(Estado estado) {
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
};

#endif