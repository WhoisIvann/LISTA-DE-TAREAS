#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include <filesystem>
#include <string>

#include "ListaTareas.h"

// Manipulacion de archivos (Fase 3):
//   - Exportar la lista de tareas a un archivo CSV que se puede abrir en Excel.
//   - Llevar un log de las acciones del usuario (agregar, editar, etc.).
//
// Solo usa la biblioteca estandar (std::ofstream): no crea ventanas.
// Si un archivo no se puede abrir o escribir, lanza ErrorArchivo
// y la ventana se encarga de avisarle al usuario.

// ---------------------------------------------------------------------------
// Exportar a CSV
// ---------------------------------------------------------------------------
class ExportadorCSV {
public:
    // Escribe todas las tareas de la lista en el archivo indicado,
    // una por renglon, con el encabezado:
    //   tipo,descripcion,estado,fecha_creacion,dato_extra,detalle
    // Lanza ErrorArchivo si no se puede abrir o escribir.
    static void exportar(const ListaTareas& lista, const std::filesystem::path& ruta);

    // Prepara un texto para ponerlo en una celda del CSV:
    // si tiene comas, comillas o saltos de linea lo encierra entre comillas
    // y duplica las comillas internas  ( Hola, "mundo"  ->  "Hola, ""mundo""" ).
    static std::string escapar(const std::string& texto);
};

// ---------------------------------------------------------------------------
// Log de acciones
// ---------------------------------------------------------------------------
class RegistroAcciones {
private:
    std::filesystem::path ruta_;

public:
    explicit RegistroAcciones(const std::filesystem::path& ruta);

    const std::filesystem::path& ruta() const;

    // Agrega al final del archivo (modo append) una linea como:
    //   2026/11/27 14:05 | Agregar | [Prioritaria] Estudiar POO
    // Lanza ErrorArchivo si no se puede abrir o escribir.
    void registrar(const std::string& accion, const std::string& descripcion) const;
};

#endif
