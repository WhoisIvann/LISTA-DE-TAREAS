#include "VentanaPrincipal.h"

#include <QAction>
#include <QBrush>
#include <QColor>
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QFont>
#include <QHeaderView>
#include <QInputDialog>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QStringList>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QToolBar>
#include <exception>

#include "DialogoTarea.h"
#include "Excepciones.h"
#include "Tarea.h"

namespace {
    // Una tarea con este peso o mas se considera urgente
    // (prioritaria Alta o con fecha vencida) y se pinta en rojo.
    constexpr int PESO_URGENTE = 3;

    // QString -> ruta de archivo. Se pasa por UTF-16 para que funcione
    // tambien con carpetas que tienen acentos o ñ en Windows.
    std::filesystem::path aRuta(const QString& texto) {
        return std::filesystem::path(texto.toStdU16String());
    }

    // El log se guarda junto al ejecutable, para encontrarlo facil en la demo.
    std::filesystem::path rutaLog() {
        return aRuta(QDir(QCoreApplication::applicationDirPath()).filePath("acciones.log"));
    }
}

VentanaPrincipal::VentanaPrincipal(BaseDatos& bd, QWidget* parent)
    : QMainWindow(parent),
      bd_(bd),
      registro_(rutaLog())
{
    setWindowTitle("Lista de tareas");
    resize(900, 500);

    crearTabla();
    crearAcciones();
    refrescar();
}

ListaTareas& VentanaPrincipal::lista() {
    return lista_;
}

// ---------------------------------------------------------------------------
// Construccion de la interfaz
// ---------------------------------------------------------------------------

void VentanaPrincipal::crearTabla() {
    tabla_ = new QTableWidget(this);
    tabla_->setColumnCount(NumColumnas);
    tabla_->setHorizontalHeaderLabels({"Tipo", "Descripción", "Estado", "Detalle"});

    // Se selecciona la fila completa y solo una a la vez.
    tabla_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla_->setSelectionMode(QAbstractItemView::SingleSelection);
    // La tabla es solo para ver; los cambios se hacen con el dialogo.
    tabla_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tabla_->verticalHeader()->setVisible(false);
    tabla_->horizontalHeader()->setSectionResizeMode(ColTipo, QHeaderView::ResizeToContents);
    tabla_->horizontalHeader()->setSectionResizeMode(ColDescripcion, QHeaderView::Stretch);
    tabla_->horizontalHeader()->setSectionResizeMode(ColEstado, QHeaderView::ResizeToContents);
    tabla_->horizontalHeader()->setSectionResizeMode(ColDetalle, QHeaderView::Stretch);

    setCentralWidget(tabla_);

    connect(tabla_, &QTableWidget::itemSelectionChanged,
            this, &VentanaPrincipal::actualizarBotones);
    // Doble clic sobre una fila = editarla.
    connect(tabla_, &QTableWidget::cellDoubleClicked,
            this, &VentanaPrincipal::editarTarea);
}

void VentanaPrincipal::crearAcciones() {
    // ---- Menu Archivo ----
    accionExportar_ = new QAction("Exportar a CSV...", this);
    accionExportar_->setShortcut(QKeySequence("Ctrl+E"));
    connect(accionExportar_, &QAction::triggered, this, &VentanaPrincipal::exportarCSV);

    QAction* accionSalir = new QAction("Salir", this);
    accionSalir->setShortcut(QKeySequence::Quit);
    connect(accionSalir, &QAction::triggered, this, &QWidget::close);

    QMenu* menuArchivo = menuBar()->addMenu("&Archivo");
    menuArchivo->addAction(accionExportar_);
    menuArchivo->addSeparator();
    menuArchivo->addAction(accionSalir);

    // ---- Menu Tareas ----
    accionAgregar_       = new QAction("Agregar", this);
    accionEditar_        = new QAction("Editar", this);
    accionCambiarEstado_ = new QAction("Cambiar estado", this);
    accionEliminar_      = new QAction("Eliminar", this);
    accionOrdenar_       = new QAction("Ordenar", this);

    accionAgregar_->setShortcut(QKeySequence::New);
    accionEliminar_->setShortcut(QKeySequence::Delete);

    connect(accionAgregar_,       &QAction::triggered, this, &VentanaPrincipal::agregarTarea);
    connect(accionEditar_,        &QAction::triggered, this, &VentanaPrincipal::editarTarea);
    connect(accionCambiarEstado_, &QAction::triggered, this, &VentanaPrincipal::cambiarEstado);
    connect(accionEliminar_,      &QAction::triggered, this, &VentanaPrincipal::eliminarTarea);
    connect(accionOrdenar_,       &QAction::triggered, this, &VentanaPrincipal::ordenarTareas);

    // Las mismas acciones aparecen en el menu y en la barra de botones.
    QMenu* menuTareas = menuBar()->addMenu("&Tareas");
    menuTareas->addAction(accionAgregar_);
    menuTareas->addAction(accionEditar_);
    menuTareas->addAction(accionCambiarEstado_);
    menuTareas->addAction(accionEliminar_);
    menuTareas->addSeparator();
    menuTareas->addAction(accionOrdenar_);

    QToolBar* barra = addToolBar("Tareas");
    barra->setMovable(false);
    barra->addAction(accionAgregar_);
    barra->addAction(accionEditar_);
    barra->addAction(accionCambiarEstado_);
    barra->addAction(accionEliminar_);
    barra->addSeparator();
    barra->addAction(accionOrdenar_);
}

// ---------------------------------------------------------------------------
// Mostrar las tareas
// ---------------------------------------------------------------------------

void VentanaPrincipal::refrescar() {
    tabla_->setRowCount(static_cast<int>(lista_.contar()));

    for (size_t i = 0; i < lista_.contar(); ++i) {
        const Tarea& tarea = lista_.obtener(i);
        const int fila = static_cast<int>(i);

        // Solo metodos virtuales / comunes: no importa si en realidad es
        // Tarea, TareaConFecha o TareaPrioritaria, cada una responde a su manera.
        tabla_->setItem(fila, ColTipo,
                        new QTableWidgetItem(QString::fromStdString(tarea.tipo())));
        tabla_->setItem(fila, ColDescripcion,
                        new QTableWidgetItem(QString::fromStdString(tarea.getDescription())));
        tabla_->setItem(fila, ColEstado,
                        new QTableWidgetItem(QString::fromStdString(
                            Tarea::estadoToString(tarea.getEstado()))));
        tabla_->setItem(fila, ColDetalle,
                        new QTableWidgetItem(QString::fromStdString(tarea.detalle())));

        // peso() tambien es virtual: una prioritaria Alta o una tarea con
        // fecha vencida regresan 3, sin que la ventana sepa cual de las dos es.
        if (tarea.peso() >= PESO_URGENTE) {
            for (int col = 0; col < NumColumnas; ++col) {
                QTableWidgetItem* celda = tabla_->item(fila, col);
                celda->setForeground(QBrush(QColor(200, 0, 0)));
                QFont fuente = celda->font();
                fuente.setBold(true);
                celda->setFont(fuente);
            }
        }
    }

    statusBar()->showMessage(QString("%1 tarea(s)").arg(lista_.contar()));
    actualizarBotones();
}

int VentanaPrincipal::filaSeleccionada() const {
    const QList<QTableWidgetItem*> seleccion = tabla_->selectedItems();
    if (seleccion.isEmpty()) {
        return -1;
    }
    return seleccion.first()->row();
}

void VentanaPrincipal::actualizarBotones() {
    // Editar, cambiar estado y eliminar necesitan una tarea seleccionada.
    const bool haySeleccion = filaSeleccionada() >= 0;
    accionEditar_->setEnabled(haySeleccion);
    accionCambiarEstado_->setEnabled(haySeleccion);
    accionEliminar_->setEnabled(haySeleccion);
    accionOrdenar_->setEnabled(lista_.contar() > 1);
}

// ---------------------------------------------------------------------------
// Manejo de errores
// ---------------------------------------------------------------------------

void VentanaPrincipal::ejecutar(const std::function<void()>& accion) {
    try {
        accion();
    }
    catch (const ErrorTareas& e) {
        // Nuestros errores (ErrorValidacion, ErrorBaseDatos, ErrorArchivo):
        // titulo() es virtual, cada uno trae su propio titulo.
        mostrarError(QString::fromStdString(e.titulo()), QString::fromStdString(e.what()));
    }
    catch (const std::exception& e) {
        mostrarError("Error inesperado", QString::fromStdString(e.what()));
    }
    catch (...) {
        mostrarError("Error inesperado", "Ocurrió un error desconocido.");
    }

    // Pase lo que pase, la tabla se vuelve a dibujar con lo que hay en la lista.
    refrescar();
}

void VentanaPrincipal::mostrarError(const QString& titulo, const QString& mensaje) {
    QMessageBox::warning(this, titulo, mensaje);
}

// ---------------------------------------------------------------------------
// Log de acciones
// ---------------------------------------------------------------------------

void VentanaPrincipal::registrar(const std::string& accion, const Tarea& tarea) {
    // tipo() es virtual: cada tarea dice de que tipo es.
    registrar(accion, "[" + tarea.tipo() + "] " + tarea.getDescription());
}

void VentanaPrincipal::registrar(const std::string& accion, const std::string& descripcion) {
    // Lanza ErrorArchivo si no se puede escribir; ejecutar() lo muestra.
    registro_.registrar(accion, descripcion);
}

// ---------------------------------------------------------------------------
// Acciones
// ---------------------------------------------------------------------------

void VentanaPrincipal::agregarTarea() {
    ejecutar([this]() {
        DialogoTarea dialogo(this);
        dialogo.setWindowTitle("Agregar tarea");

        if (dialogo.exec() == QDialog::Accepted) {
            // Lanza ErrorValidacion si ya existe una tarea igual.
            lista_.agregarTarea(dialogo.crearTarea());

            // Guardarla en la base de datos y quedarse con su id.
            // Si no se pudo guardar, se quita de la lista para que la
            // tabla y la base de datos no queden diferentes.
            const size_t nueva = lista_.contar() - 1;
            try {
                lista_.asignarId(nueva, bd_.guardar(lista_.obtener(nueva)));
            }
            catch (...) {
                lista_.eliminarTarea(nueva);
                throw;  // ejecutar() muestra el error
            }

            refrescar();
            // Dejar seleccionada la tarea recien agregada (queda al final).
            tabla_->selectRow(static_cast<int>(lista_.contar()) - 1);

            registrar("Agregar", lista_.obtener(lista_.contar() - 1));
        }
    });
}

void VentanaPrincipal::editarTarea() {
    const int fila = filaSeleccionada();
    if (fila < 0) {
        return;
    }

    ejecutar([this, fila]() {
        DialogoTarea dialogo(this);
        dialogo.setWindowTitle("Editar tarea");
        dialogo.cargar(lista_.obtener(static_cast<size_t>(fila)));

        if (dialogo.exec() == QDialog::Accepted) {
            // Lanza ErrorValidacion si quedaria repetida con otra tarea.
            lista_.editarTarea(static_cast<size_t>(fila), dialogo.crearTarea());
            // editarTarea() conserva el id, asi se actualiza la misma fila.
            bd_.actualizar(lista_.obtener(static_cast<size_t>(fila)));
            refrescar();
            tabla_->selectRow(fila);

            registrar("Editar", lista_.obtener(static_cast<size_t>(fila)));
        }
    });
}

void VentanaPrincipal::cambiarEstado() {
    const int fila = filaSeleccionada();
    if (fila < 0) {
        return;
    }

    ejecutar([this, fila]() {
        const Estado estados[] = {Estado::Pendiente, Estado::EnProgreso, Estado::Completada};

        QStringList opciones;
        for (Estado e : estados) {
            opciones << QString::fromStdString(Tarea::estadoToString(e));
        }

        const Tarea& tarea = lista_.obtener(static_cast<size_t>(fila));
        const int actual = static_cast<int>(tarea.getEstado()) - 1;  // Pendiente = 1

        bool ok = false;
        const QString elegido = QInputDialog::getItem(
            this, "Cambiar estado",
            QString("Nuevo estado de \"%1\":").arg(QString::fromStdString(tarea.getDescription())),
            opciones, actual, false, &ok);

        if (ok) {
            const int indice = static_cast<int>(opciones.indexOf(elegido));
            lista_.cambiarEstado(static_cast<size_t>(fila), estados[indice]);
            bd_.actualizar(lista_.obtener(static_cast<size_t>(fila)));
            refrescar();
            tabla_->selectRow(fila);

            // Ej. "Cambiar estado a Completada" (cuenta como "completar" la tarea).
            registrar("Cambiar estado a " + Tarea::estadoToString(estados[indice]),
                      lista_.obtener(static_cast<size_t>(fila)));
        }
    });
}

void VentanaPrincipal::eliminarTarea() {
    const int fila = filaSeleccionada();
    if (fila < 0) {
        return;
    }

    ejecutar([this, fila]() {
        const Tarea& tarea = lista_.obtener(static_cast<size_t>(fila));

        // Confirmar antes de borrar: no se puede deshacer.
        const QMessageBox::StandardButton respuesta = QMessageBox::question(
            this, "Eliminar tarea",
            QString("¿Seguro que quieres eliminar \"%1\"?")
                .arg(QString::fromStdString(tarea.getDescription())),
            QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

        if (respuesta == QMessageBox::Yes) {
            // Se guarda el texto antes de borrar: despues la tarea ya no existe.
            const std::string eliminada = "[" + tarea.tipo() + "] " + tarea.getDescription();
            // Primero de la base de datos: si falla, la tarea se queda en la lista.
            bd_.eliminar(tarea.getId());
            lista_.eliminarTarea(static_cast<size_t>(fila));
            registrar("Eliminar", eliminada);
        }
    });
}

void VentanaPrincipal::ordenarTareas() {
    ejecutar([this]() {
        // Usa operator< de Tarea: primero prioridad (peso) y despues fecha.
        lista_.ordenar();
    });
}

void VentanaPrincipal::exportarCSV() {
    ejecutar([this]() {
        const QString archivo = QFileDialog::getSaveFileName(
            this, "Exportar tareas a CSV",
            QDir::home().filePath("tareas.csv"),
            "Archivo CSV (*.csv)");

        if (archivo.isEmpty()) {
            return;  // el usuario cancelo
        }

        // Lanza ErrorArchivo si no se puede escribir; ejecutar() lo muestra.
        ExportadorCSV::exportar(lista_, aRuta(archivo));

        registrar("Exportar CSV", archivo.toStdString());
        statusBar()->showMessage(QString("Se exportaron %1 tarea(s) a %2")
                                     .arg(lista_.contar()).arg(archivo), 5000);
    });
}
