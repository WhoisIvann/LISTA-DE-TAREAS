#ifndef APLICACION_H
#define APLICACION_H

#include <QApplication>

// Nuestra aplicacion: hereda de QApplication y sobrescribe notify().
//
// Qt llama a notify() (metodo virtual) para entregar cada evento: clics,
// teclas, etc. Si un slot (por ejemplo el boton "Agregar") lanza una
// excepcion que nadie atrapo, aqui la atrapamos, la mostramos en un
// QMessageBox y la aplicacion sigue funcionando en lugar de cerrarse.
class Aplicacion : public QApplication {
public:
    Aplicacion(int& argc, char** argv);

    bool notify(QObject* receptor, QEvent* evento) override;

    // Muestra un error al usuario. Se puede usar desde cualquier parte.
    static void mostrarError(const QString& titulo, const QString& mensaje);
};

#endif
