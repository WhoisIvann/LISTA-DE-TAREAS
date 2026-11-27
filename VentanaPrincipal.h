#ifndef VENTANA_PRINCIPAL_H
#define VENTANA_PRINCIPAL_H

#include <QMainWindow>
#include <functional>
#include <string>

#include "Archivos.h"
#include "BaseDatos.h"
#include "ListaTareas.h"

class QAction;
class QTableWidget;

// Ventana principal de la aplicacion: hereda de QMainWindow.
//
// Muestra todas las tareas en una tabla y permite agregarlas, editarlas,
// cambiar su estado, eliminarlas y ordenarlas.
//
// Para llenar la tabla solo se usan los metodos virtuales de Tarea
// (tipo(), detalle(), ...), sin preguntar con if/switch de que tipo es
// cada tarea: cada clase derivada responde a su manera (polimorfismo).
//
// Cada accion (agregar, editar, cambiar estado, eliminar) tambien se guarda
// en la base de datos, asi las tareas siguen ahi al volver a abrir la app.
class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

public:
    // bd: base de datos ya conectada donde se guardan los cambios.
    explicit VentanaPrincipal(BaseDatos& bd, QWidget* parent = nullptr);

    // La lista que muestra la ventana (la base de datos la llena al abrir).
    ListaTareas& lista();

    // Vuelve a dibujar la tabla con el contenido actual de la lista.
    void refrescar();

private slots:
    void agregarTarea();
    void editarTarea();
    void cambiarEstado();
    void eliminarTarea();
    void ordenarTareas();
    void exportarCSV();
    void actualizarBotones();

private:
    // Columnas de la tabla.
    enum Columna { ColTipo = 0, ColDescripcion, ColEstado, ColDetalle, NumColumnas };

    ListaTareas lista_;
    BaseDatos& bd_;
    QTableWidget* tabla_;

    // Log de acciones (acciones.log, junto al ejecutable).
    RegistroAcciones registro_;

    QAction* accionExportar_;
    QAction* accionAgregar_;
    QAction* accionEditar_;
    QAction* accionCambiarEstado_;
    QAction* accionEliminar_;
    QAction* accionOrdenar_;

    void crearTabla();
    void crearAcciones();

    // Indice (en la lista) de la fila seleccionada, o -1 si no hay ninguna.
    int filaSeleccionada() const;

    // Ejecuta una accion dentro de try/catch. Si lanza una excepcion,
    // la muestra en un QMessageBox y la aplicacion sigue funcionando.
    void ejecutar(const std::function<void()>& accion);

    // Muestra un error al usuario con un QMessageBox::warning.
    void mostrarError(const QString& titulo, const QString& mensaje);

    // Escribe una accion en el log, por ejemplo: "Agregar | [Prioritaria] Estudiar".
    void registrar(const std::string& accion, const Tarea& tarea);
    void registrar(const std::string& accion, const std::string& descripcion);
};

#endif
