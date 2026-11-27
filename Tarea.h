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
    int id_;                       // id en la base de datos (0 = aun no se guarda)
    std::string description_;
    Estado estado_;
    std::chrono::system_clock::time_point creationDate_;

public:
    // Lanza ErrorValidacion si la descripcion esta vacia.
    Tarea(const std::string& description,
          const std::chrono::system_clock::time_point& creationDate);

    // Destructor virtual: necesario porque la lista guarda
    // punteros a Tarea que en realidad apuntan a clases derivadas.
    virtual ~Tarea() = default;

    int getId() const;
    std::string getDescription() const;
    Estado getEstado() const;
    std::string getCreationDate() const;
    std::chrono::system_clock::time_point getCreationTimePoint() const;

    void setId(int id);
    // Lanza ErrorValidacion si la descripcion esta vacia.
    void setDescription(const std::string& description);
    void setEstado(Estado estado);

    // ---- Comportamiento que cada clase derivada puede sobrescribir ----

    // Nombre del tipo de tarea ("Normal", "Con fecha", "Prioritaria").
    virtual std::string tipo() const;

    // Texto con los datos propios del tipo ("Limite: ...", "Urgencia: ...").
    // La ventana lo usa para mostrar cualquier tarea sin preguntar su tipo.
    // En una tarea normal regresa "".
    virtual std::string detalle() const;

    // Dato extra del tipo, en forma simple para guardarlo en la base de datos
    // o en un CSV: segundos de la fecha limite, numero de urgencia, o "".
    virtual std::string datoExtra() const;

    // Imprime la tarea. Las derivadas agregan sus propios datos.
    virtual void mostrar(std::ostream& os) const;

    // La tarea completa como texto (lo mismo que imprime mostrar()).
    std::string texto() const;

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

    // Segundos desde 1970 <-> time_point (para guardar fechas en la BD).
    static long long aSegundos(const std::chrono::system_clock::time_point& tp);
    static std::chrono::system_clock::time_point desdeSegundos(long long segundos);
};

// cout << tarea  ->  llama a mostrar(), que es virtual.
std::ostream& operator<<(std::ostream& os, const Tarea& tarea);

#endif
