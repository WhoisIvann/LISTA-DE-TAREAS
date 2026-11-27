#pragma once

#include <QDialog>
#include <memory>

using namespace std;

class QComboBox;
class QDateTimeEdit;
class QDialogButtonBox;
class QLineEdit;

class Tarea;

class DialogoTarea : public QDialog
{
    Q_OBJECT

public:
    explicit DialogoTarea(QWidget* parent = nullptr);
    explicit DialogoTarea(const Tarea& tarea, QWidget* parent = nullptr);

    unique_ptr<Tarea> crearTarea() const;

private slots:
    void actualizarCamposPorTipo();

private:
    void configurarInterfaz();
    void cargar(const Tarea& tarea);
    bool validar() const;

    QComboBox* comboTipo;
    QLineEdit* editDescripcion;
    QComboBox* comboEstado;
    QDateTimeEdit* editFechaLimite;
    QComboBox* comboUrgencia;
    QDialogButtonBox* botones;
};