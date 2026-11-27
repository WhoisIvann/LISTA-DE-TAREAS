#ifndef EXCEPCIONES_H
#define EXCEPCIONES_H

#include <stdexcept>
#include <string>

// Jerarquia de excepciones del proyecto.
// Todas heredan de ErrorTareas, asi que con un solo
// catch (const ErrorTareas&) se atrapa cualquier error nuestro,
// y con catch (const std::exception&) cualquier error en general.
//
// Que excepcion usar en cada modulo:
//   - ErrorValidacion: datos invalidos (descripcion vacia, tarea repetida,
//                      indice fuera de rango, fecha invalida). Modelo y dialogo.
//   - ErrorBaseDatos:  no abre la BD, falla un SELECT/INSERT/UPDATE/DELETE.
//   - ErrorArchivo:    no se puede abrir o escribir el CSV o el log.
class ErrorTareas : public std::runtime_error {
public:
    explicit ErrorTareas(const std::string& mensaje)
        : std::runtime_error(mensaje) {}

    // Titulo corto para mostrar en el QMessageBox.
    virtual std::string titulo() const { return "Error"; }
};

class ErrorValidacion : public ErrorTareas {
public:
    explicit ErrorValidacion(const std::string& mensaje)
        : ErrorTareas(mensaje) {}

    std::string titulo() const override { return "Dato invalido"; }
};

class ErrorBaseDatos : public ErrorTareas {
public:
    explicit ErrorBaseDatos(const std::string& mensaje)
        : ErrorTareas(mensaje) {}

    std::string titulo() const override { return "Error de base de datos"; }
};

class ErrorArchivo : public ErrorTareas {
public:
    explicit ErrorArchivo(const std::string& mensaje)
        : ErrorTareas(mensaje) {}

    std::string titulo() const override { return "Error de archivo"; }
};

#endif
