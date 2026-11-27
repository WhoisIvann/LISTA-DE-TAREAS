// Prueba de la base de datos (#26 y #27), sin ventanas.
// Crea un archivo prueba_tareas.db, guarda una tarea de cada tipo,
// las vuelve a leer y revisa que todo coincida. Al final borra el archivo.
//
// Se corre desde la carpeta de compilacion:   ./PruebaBaseDatos

#include <QCoreApplication>
#include <QFile>
#include <chrono>
#include <iostream>
#include <memory>

#include "BaseDatos.h"
#include "Excepciones.h"
#include "ListaTareas.h"
#include "TareaConFecha.h"
#include "TareaPrioritaria.h"

using namespace std;

static int fallas = 0;

static void revisar(bool condicion, const string& que) {
    cout << (condicion ? "  OK    " : "  FALLA ") << que << endl;
    if (!condicion) {
        ++fallas;
    }
}

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    const QString archivo = "prueba_tareas.db";
    QFile::remove(archivo);  // empezar desde cero

    try {
        const auto ahora = chrono::system_clock::now();
        const auto limite = ahora + chrono::hours(48);

        {
            BaseDatos bd(archivo);
            bd.conectar();
            cout << "1) Conexion y tabla" << endl;
            revisar(bd.estaConectada(), "la base de datos se abrio");

            cout << "2) Guardar una tarea de cada tipo" << endl;
            ListaTareas lista;
            lista.agregarTarea(make_unique<Tarea>("Comprar cuaderno", ahora));
            lista.agregarTarea(make_unique<TareaConFecha>("Entregar practica \"Qt\"", ahora, limite));
            lista.agregarTarea(make_unique<TareaPrioritaria>("Estudiar POO", ahora, Urgencia::Alta));

            for (size_t i = 0; i < lista.contar(); ++i) {
                lista.asignarId(i, bd.guardar(lista.obtener(i)));
            }
            revisar(lista.obtener(0).getId() == 1 && lista.obtener(2).getId() == 3,
                    "se asignaron los ids 1, 2 y 3");

            cout << "3) Actualizar y eliminar" << endl;
            lista.cambiarEstado(1, Estado::Completada);
            bd.actualizar(lista.obtener(1));
            bd.eliminar(lista.obtener(0).getId());
            revisar(true, "actualizar() y eliminar() no lanzaron error");
        }   // aqui se cierra la base de datos, como si se cerrara la app

        cout << "4) Volver a abrir y cargar" << endl;
        BaseDatos bd(archivo);
        bd.conectar();
        ListaTareas cargada;
        bd.cargarTodas(cargada);

        revisar(cargada.contar() == 2, "quedaron 2 tareas (se elimino la normal)");
        if (cargada.contar() == 2) {
            const Tarea& conFecha = cargada.obtener(0);
            const Tarea& prioritaria = cargada.obtener(1);

            revisar(conFecha.tipo() == "Con fecha", "la primera es Con fecha");
            revisar(conFecha.getDescription() == "Entregar practica \"Qt\"",
                    "la descripcion con comillas se guardo igual");
            revisar(conFecha.getEstado() == Estado::Completada, "conserva el estado Completada");
            revisar(conFecha.datoExtra() == to_string(Tarea::aSegundos(limite)),
                    "conserva la fecha limite");
            revisar(conFecha.getId() == 2, "conserva su id");

            revisar(prioritaria.tipo() == "Prioritaria", "la segunda es Prioritaria");
            revisar(prioritaria.datoExtra() == "3", "conserva la urgencia Alta");
            revisar(prioritaria.getEstado() == Estado::Pendiente, "sigue Pendiente");
        }

        cout << "5) Errores" << endl;
        try {
            Tarea sinGuardar("No guardada", ahora);
            bd.actualizar(sinGuardar);
            revisar(false, "actualizar una tarea sin id lanza ErrorBaseDatos");
        }
        catch (const ErrorBaseDatos&) {
            revisar(true, "actualizar una tarea sin id lanza ErrorBaseDatos");
        }

        cout << endl;
        for (size_t i = 0; i < cargada.contar(); ++i) {
            cout << "  " << cargada.obtener(i) << endl;
        }
    }
    catch (const ErrorTareas& e) {
        cout << e.titulo() << ": " << e.what() << endl;
        ++fallas;
    }

    QFile::remove(archivo);

    cout << endl << (fallas == 0 ? "Todas las pruebas pasaron." : "Hubo fallas.") << endl;
    return fallas == 0 ? 0 : 1;
}
