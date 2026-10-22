#ifndef TAREA_CON_FECHA_H
#define TAREA_CON_FECHA_H

#include "Tarea.h"

// Tarea que tiene una fecha limite de entrega.
class TareaConFecha : public Tarea {
private:
    std::chrono::system_clock::time_point fechaLimite_;

public:
    TareaConFecha(const std::string& description,
                  const std::chrono::system_clock::time_point& creationDate,
                  const std::chrono::system_clock::time_point& fechaLimite);

    std::chrono::system_clock::time_point getFechaLimite() const;
    void setFechaLimite(const std::chrono::system_clock::time_point& fechaLimite);

    // true si ya paso la fecha limite y la tarea no esta completada.
    bool estaVencida() const;

    // Sobrescritos de la clase base
    std::string tipo() const override;
    void mostrar(std::ostream& os) const override;
    int peso() const override;
    std::chrono::system_clock::time_point fechaReferencia() const override;
};

#endif
