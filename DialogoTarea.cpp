#include "DialogoTarea.h"

#include <QComboBox>
#include <QDateTimeEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

using namespace std;

DialogoTarea::DialogoTarea(QWidget* parent)
    : QDialog(parent),
      comboTipo(new QComboBox(this)),
      editDescripcion(new QLineEdit(this)),
      comboEstado(new QComboBox(this)),
      editFechaLimite(new QDateTimeEdit(this)),
      comboUrgencia(new QComboBox(this)),
      botones(new QDialogButtonBox(
          QDialogButtonBox::Ok or QDialogButtonBox::Cancel, this))
{
    configurarInterfaz();

    connect(comboTipo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &DialogoTarea::actualizarCamposPorTipo);

    connect(botones, &QDialogButtonBox::accepted, this, &DialogoTarea::accept);
    connect(botones, &QDialogButtonBox::rejected, this, &DialogoTarea::reject);

    actualizarCamposPorTipo();
}

DialogoTarea::DialogoTarea(const Tarea& tarea, QWidget* parent)
    : DialogoTarea(parent)
{
    cargar(tarea);
}

void DialogoTarea::configurarInterfaz()
{
    comboTipo->addItems({
        QStringLiteral("Normal"),
        QStringLiteral("Con fecha"),
        QStringLiteral("Prioritaria")
    });

    comboEstado->addItems({
        QStringLiteral("Pendiente"),
        QStringLiteral("En progreso"),
        QStringLiteral("Completada")
    });

    comboUrgencia->addItems({
        QStringLiteral("Baja"),
        QStringLiteral("Media"),
        QStringLiteral("Alta")
    });

    editFechaLimite->setCalendarPopup(true);
    editFechaLimite->setDateTime(QDateTime::currentDateTime());

    auto* formulario = new QFormLayout;
    formulario->addRow(QStringLiteral("Tipo:"), comboTipo);
    formulario->addRow(QStringLiteral("Descripción:"), editDescripcion);
    formulario->addRow(QStringLiteral("Estado:"), comboEstado);
    formulario->addRow(QStringLiteral("Fecha límite:"), editFechaLimite);
    formulario->addRow(QStringLiteral("Urgencia:"), comboUrgencia);

    auto* diseño = new QVBoxLayout(this);
    diseño->addLayout(formulario);
    diseño->addWidget(botones);

    setLayout(diseño);
    setWindowTitle(QStringLiteral("Tarea"));
}

void DialogoTarea::actualizarCamposPorTipo()
{
    const QString tipo = comboTipo->currentText();

    const bool tieneFecha =
        tipo == QStringLiteral("Con fecha") ||
        tipo == QStringLiteral("Prioritaria");

    const bool tieneUrgencia =
        tipo == QStringLiteral("Prioritaria");

    editFechaLimite->setVisible(tieneFecha);
    comboUrgencia->setVisible(tieneUrgencia);

    editFechaLimite->parentWidget()->layout()->itemAt(3)->widget()
        ->setVisible(tieneFecha);

    comboUrgencia->parentWidget()->layout()->itemAt(4)->widget()
        ->setVisible(tieneUrgencia);
}

bool DialogoTarea::validar() const
{
    if (editDescripcion->text().trimmed().isEmpty()) {
        QMessageBox::warning(
            const_cast<DialogoTarea*>(this),
            QStringLiteral("Datos incorrectos"),
            QStringLiteral("La descripción no puede estar vacía."));

        return false;
    }

    if (editFechaLimite->isVisible() &&
        !editFechaLimite->dateTime().isValid()) {
        QMessageBox::warning(
            const_cast<DialogoTarea*>(this),
            QStringLiteral("Datos incorrectos"),
            QStringLiteral("La fecha límite no es válida."));

        return false;
    }

    return true;
}

void DialogoTarea::accept()
{
    if (!validar()) {
        return;
    }

    QDialog::accept();
}

TareaNormal(const string& descripcion, Estado estado);
TareaConFecha(const string& descripcion, Estado estado, const QDateTime& fecha);
TareaPrioritaria(const string& descripcion, Estado estado, const QDateTime& fecha, Urgencia urgencia);