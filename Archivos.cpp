#include "Archivos.h"

#include <chrono>
#include <fstream>

#include "Excepciones.h"
#include "Tarea.h"

using namespace std;

// ---------------------------------------------------------------------------
// ExportadorCSV
// ---------------------------------------------------------------------------

string ExportadorCSV::escapar(const string& texto) {
    const bool necesitaComillas = texto.find_first_of(",\"\r\n") != string::npos;
    if (!necesitaComillas) {
        return texto;
    }

    string resultado = "\"";
    for (char c : texto) {
        if (c == '"') {
            resultado += "\"\"";  // una comilla dentro del texto se escribe doble
        } else {
            resultado += c;
        }
    }
    resultado += "\"";
    return resultado;
}

void ExportadorCSV::exportar(const ListaTareas& lista, const filesystem::path& ruta) {
    ofstream archivo(ruta, ios::out | ios::trunc | ios::binary);
    if (!archivo.is_open()) {
        throw ErrorArchivo("No se pudo crear el archivo:\n" + ruta.string());
    }

    // Marca BOM de UTF-8: asi Excel muestra bien los acentos y la ñ.
    archivo << "\xEF\xBB\xBF";

    archivo << "tipo,descripcion,estado,fecha_creacion,dato_extra,detalle\r\n";

    for (size_t i = 0; i < lista.contar(); ++i) {
        const Tarea& tarea = lista.obtener(i);

        // Igual que en la ventana: solo metodos virtuales, sin revisar el tipo.
        archivo << escapar(tarea.tipo()) << ','
                << escapar(tarea.getDescription()) << ','
                << escapar(Tarea::estadoToString(tarea.getEstado())) << ','
                << escapar(tarea.getCreationDate()) << ','
                << escapar(tarea.datoExtra()) << ','
                << escapar(tarea.detalle()) << "\r\n";
    }

    archivo.flush();
    if (!archivo) {
        throw ErrorArchivo("Ocurrió un error al escribir el archivo:\n" + ruta.string());
    }
}

// ---------------------------------------------------------------------------
// RegistroAcciones
// ---------------------------------------------------------------------------

RegistroAcciones::RegistroAcciones(const filesystem::path& ruta)
    : ruta_(ruta)
{
}

const filesystem::path& RegistroAcciones::ruta() const {
    return ruta_;
}

void RegistroAcciones::registrar(const string& accion, const string& descripcion) const {
    // ios::app = agregar al final sin borrar lo que ya tenia el archivo.
    ofstream archivo(ruta_, ios::out | ios::app);
    if (!archivo.is_open()) {
        throw ErrorArchivo("No se pudo abrir el log de acciones:\n" + ruta_.string());
    }

    archivo << Tarea::formatearFecha(chrono::system_clock::now())
            << " | " << accion
            << " | " << descripcion << '\n';

    if (!archivo) {
        throw ErrorArchivo("No se pudo escribir en el log de acciones:\n" + ruta_.string());
    }
}
