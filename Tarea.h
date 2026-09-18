#ifndef TAREA_H
#define TAREA_H

#include <string>

// Estado posible de una tarea.
enum class Estado {
    PENDIENTE,
    EN_PROGRESO,
    COMPLETADA
};

// Convierte un Estado a texto legible, y viceversa (para el menú).
std::string estadoATexto(Estado estado);
Estado textoAEstado(int opcion); // 1=PENDIENTE, 2=EN_PROGRESO, 3=COMPLETADA

// Representa una tarea individual dentro de la lista de tareas.
// Atributos privados: solo se accede/modifica a través de los métodos públicos,
// lo que permite validar datos y mantener invariantes de la clase.
class Tarea {
private:
    int id;
    std::string descripcion;
    Estado estado;
    std::string fechaCreacion; // formato: AAAA-MM-DD HH:MM

public:
    // Constructor: la fecha de creación se calcula automáticamente.
    Tarea(int id, const std::string& descripcion);

    // --- Getters ---
    int getId() const;
    std::string getDescripcion() const;
    Estado getEstado() const;
    std::string getFechaCreacion() const;

    // --- Setters con validación ---
    // Devuelve false si la descripción es inválida (vacía) y no modifica el atributo.
    bool setDescripcion(const std::string& nuevaDescripcion);
    void setEstado(Estado nuevoEstado);

    // Representación en texto de la tarea, lista para imprimir en el menú.
    std::string toString() const;
};

#endif // TAREA_H

