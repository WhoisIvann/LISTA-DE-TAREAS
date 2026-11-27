// Lista de tareas - Fase 3: aplicacion de escritorio con Qt.

#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <exception>

#include "Aplicacion.h"
#include "Excepciones.h"

int main(int argc, char* argv[]) {
    Aplicacion app(argc, argv);

    try {
        // TODO (Gerardo, #22): reemplazar esta ventana por VentanaPrincipal.
        // TODO (Augusto, #26/#27): conectar la base de datos y cargar las tareas
        //      antes de mostrar la ventana; si falla, lanza ErrorBaseDatos y
        //      se informa abajo en lugar de cerrar de golpe.
        QMainWindow ventana;
        ventana.setWindowTitle("Lista de tareas");
        ventana.setCentralWidget(new QLabel("Hola mundo desde Qt: Lista de tareas, Fase 3"));
        ventana.resize(640, 400);
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
