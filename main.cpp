// Lista de tareas - Fase 3: aplicacion de escritorio con Qt.

#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>
#include <exception>

#include "Aplicacion.h"
#include "BaseDatos.h"
#include "Excepciones.h"
#include "VentanaPrincipal.h"

int main(int argc, char* argv[]) {
    Aplicacion app(argc, argv);

    try {
        // La base de datos (tareas.db) va junto al ejecutable, igual que acciones.log.
        // Si no se puede abrir o leer, lanza ErrorBaseDatos y se avisa abajo
        // en lugar de cerrar de golpe.
        BaseDatos bd(QDir(QCoreApplication::applicationDirPath()).filePath("tareas.db"));
        bd.conectar();

        VentanaPrincipal ventana(bd);
        bd.cargarTodas(ventana.lista());   // las tareas guardadas la ultima vez
        ventana.refrescar();
        ventana.show();

        return app.exec();
    }
    catch (const ErrorTareas& e) {
        QMessageBox::critical(nullptr, QString::fromStdString(e.titulo()),
                              QString::fromStdString(e.what()));
    }
    catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Error inesperado", QString::fromStdString(e.what()));
    }
    return 1;
}
