#ifndef TAREA_PRIORITARIA_H
#define TAREA_PRIORITARIA_H

#include "Tarea.h"

enum class Urgencia {
    Baja = 1,
    Media = 2,
    Alta = 3
};

// Tarea con un nivel de urgencia.
class TareaPrioritaria : public Tarea {
private:
    Urgencia urgencia_;

public:
    TareaPrioritaria(const std::string& description,
                     const std::chrono::system_clock::time_point& creationDate,
                     Urgencia urgencia);

    Urgencia getUrgencia() const;
    void setUrgencia(Urgencia urgencia);

    static std::string urgenciaToString(Urgencia urgencia);

    // Sobrescritos de la clase base
    std::string tipo() const override;
    void mostrar(std::ostream& os) const override;
    int peso() const override;
};

#endif
