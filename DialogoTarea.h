#ifndef DIALOGO_TAREA_H
#define DIALOGO_TAREA_H

#include <QDialog>
#include <chrono>
#include <memory>

class QComboBox;
class QDateTimeEdit;
class QDialogButtonBox;
class QLabel;
class QLineEdit;

class Tarea;

// Cuadro de dialogo para capturar o editar una tarea: hereda de QDialog.
//
// Permite elegir el tipo (Normal, Con fecha, Prioritaria) y muestra
// solo los campos de ese tipo. Sobrescribe accept() para validar los
// datos antes de cerrarse: si algo esta mal, avisa con un QMessageBox
// y el dialogo se queda abierto.
class DialogoTarea : public QDialog {
    Q_OBJECT

public:
    // Dialogo vacio, para agregar una tarea nueva.
    explicit DialogoTarea(QWidget* parent = nullptr);

    // Dialogo con los datos de una tarea existente, para editarla.
    explicit DialogoTarea(const Tarea& tarea, QWidget* parent = nullptr);

    // Llena los campos con los datos de una tarea (modo edicion).
    // Conserva su fecha de creacion al crear la tarea editada.
    void cargar(const Tarea& tarea);

    // Crea la tarea del tipo elegido con los datos capturados.
    // Llamarla despues de que exec() regrese QDialog::Accepted.
    // Lanza ErrorValidacion si los datos no son validos.
    std::unique_ptr<Tarea> crearTarea() const;

public slots:
    // Metodo virtual de QDialog: se llama al presionar Aceptar.
    // Solo cierra el dialogo si los datos son validos.
    void accept() override;

private slots:
    void actualizarCamposPorTipo();

private:
    // Posicion de cada tipo en el combo.
    enum IndiceTipo { TipoNormal = 0, TipoConFecha = 1, TipoPrioritaria = 2 };

    QComboBox* comboTipo_;
    QLineEdit* editDescripcion_;
    QComboBox* comboEstado_;
    QLabel* etiquetaFecha_;
    QDateTimeEdit* editFechaLimite_;
    QLabel* etiquetaUrgencia_;
    QComboBox* comboUrgencia_;
    QDialogButtonBox* botones_;

    // Fecha de creacion de la tarea (ahora, o la original al editar).
    std::chrono::system_clock::time_point fechaCreacion_;

    void configurarInterfaz();
};

#endif
