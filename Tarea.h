#ifndef TAREA_H
#define TAREA_H

#include <chrono>
#include <iostream>
#include <string>

enum class Estado {
    Pendiente = 1,
    EnProgreso = 2,
    Completada = 3
};

// Clase base de todas las tareas.
// Los metodos virtuales permiten que cada tipo de tarea
// se muestre y se ordene a su manera (polimorfismo).
class Tarea {
private:
    std::string description_;
    Estado estado_;
    std::chrono::system_clock::time_point creationDate_;

public:
    Tarea(const std::string& description,
          const std::chrono::system_clock::time_point& creationDate);

    // Destructor virtual: necesario porque la lista guarda
    // punteros a Tarea que en realidad apuntan a clases derivadas.
    virtual ~Tarea() = default;

    std::string getDescription() const;
    Estado getEstado() const;
    std::string getCreationDate() const;
    std::chrono::system_clock::time_point getCreationTimePoint() const;

    void setDescription(const std::string& description);
    void setEstado(Estado estado);

    // ---- Comportamiento que cada clase derivada puede sobrescribir ----

    // Nombre del tipo de tarea ("Normal", "Con fecha", "Prioritaria").
    virtual std::string tipo() const;

    // Imprime la tarea. Las derivadas agregan sus propios datos.
    virtual void mostrar(std::ostream& os) const;

    // Peso para ordenar: mientras mas alto, mas arriba en la lista.
    virtual int peso() const;

    // Fecha que se usa para desempatar al ordenar.
    virtual std::chrono::system_clock::time_point fechaReferencia() const;

    // ---- Sobrecarga de operadores ----

    // a < b  significa "a va antes que b" al ordenar:
    // primero por peso (prioridad) y luego por fecha.
    bool operator<(const Tarea& otra) const;

    // Dos tareas son iguales si son del mismo tipo y tienen la
    // misma descripcion (se usa para evitar duplicados).
    bool operator==(const Tarea& otra) const;

    static std::string estadoToString(Estado estado);
    static std::string formatearFecha(const std::chrono::system_clock::time_point& tp);
};

// cout << tarea  ->  llama a mostrar(), que es virtual.
std::ostream& operator<<(std::ostream& os, const Tarea& tarea);

#endif
