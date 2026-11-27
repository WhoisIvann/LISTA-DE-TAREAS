#include "Aplicacion.h"

#include <QMessageBox>
#include <exception>

#include "Excepciones.h"

Aplicacion::Aplicacion(int& argc, char** argv)
    : QApplication(argc, argv)
{
    setApplicationName("Lista de tareas");
}

bool Aplicacion::notify(QObject* receptor, QEvent* evento) {
    try {
        return QApplication::notify(receptor, evento);
    }
    catch (const ErrorTareas& e) {
        mostrarError(QString::fromStdString(e.titulo()), QString::fromStdString(e.what()));
    }
    catch (const std::exception& e) {
        mostrarError("Error inesperado", QString::fromStdString(e.what()));
    }
    catch (...) {
        mostrarError("Error inesperado", "Ocurrio un error desconocido.");
    }
    return false;
}

void Aplicacion::mostrarError(const QString& titulo, const QString& mensaje) {
    QMessageBox::warning(nullptr, titulo, mensaje);
}
