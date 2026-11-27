#ifndef VENTANA_PRINCIPAL_H
#define VENTANA_PRINCIPAL_H

#include <QMainWindow>

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
class VentanaPrincipal : public QMainWindow {
    Q_OBJECT

public:
    explicit VentanaPrincipal(QWidget* parent = nullptr);

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
    void actualizarBotones();

private:
    // Columnas de la tabla.
    enum Columna { ColTipo = 0, ColDescripcion, ColEstado, ColDetalle, NumColumnas };

    ListaTareas lista_;
    QTableWidget* tabla_;

    QAction* accionAgregar_;
    QAction* accionEditar_;
    QAction* accionCambiarEstado_;
    QAction* accionEliminar_;
    QAction* accionOrdenar_;

    void crearTabla();
    void crearAcciones();

    // Indice (en la lista) de la fila seleccionada, o -1 si no hay ninguna.
    int filaSeleccionada() const;
};

#endif
