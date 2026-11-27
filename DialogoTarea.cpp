#include "DialogoTarea.h"

#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QVBoxLayout>
#include <string>

#include "Excepciones.h"
#include "Tarea.h"
#include "TareaConFecha.h"
#include "TareaPrioritaria.h"

using namespace std;

namespace {
    // Conversiones entre las fechas de Qt y las de la biblioteca estandar.
    chrono::system_clock::time_point aTimePoint(const QDateTime& fecha) {
        return Tarea::desdeSegundos(fecha.toSecsSinceEpoch());
    }

    QDateTime aQDateTime(const chrono::system_clock::time_point& tp) {
        return QDateTime::fromSecsSinceEpoch(Tarea::aSegundos(tp));
    }
}

DialogoTarea::DialogoTarea(QWidget* parent)
    : QDialog(parent),
      comboTipo_(new QComboBox(this)),
      editDescripcion_(new QLineEdit(this)),
      comboEstado_(new QComboBox(this)),
      etiquetaFecha_(new QLabel("Fecha límite:", this)),
      editFechaLimite_(new QDateTimeEdit(this)),
      etiquetaUrgencia_(new QLabel("Urgencia:", this)),
      comboUrgencia_(new QComboBox(this)),
      botones_(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this)),
      fechaCreacion_(chrono::system_clock::now())
{
    configurarInterfaz();

    connect(comboTipo_, &QComboBox::currentIndexChanged,
            this, &DialogoTarea::actualizarCamposPorTipo);

    // Aceptar llama a accept() (sobrescrito abajo); Cancelar a reject().
    connect(botones_, &QDialogButtonBox::accepted, this, &DialogoTarea::accept);
    connect(botones_, &QDialogButtonBox::rejected, this, &DialogoTarea::reject);

    actualizarCamposPorTipo();
}

DialogoTarea::DialogoTarea(const Tarea& tarea, QWidget* parent)
    : DialogoTarea(parent)
{
    cargar(tarea);
}

void DialogoTarea::configurarInterfaz() {
    setWindowTitle("Tarea");

    // El orden debe coincidir con IndiceTipo.
    comboTipo_->addItems({"Normal", "Con fecha", "Prioritaria"});

    // El orden coincide con el enum Estado (Pendiente = 1, ...).
    comboEstado_->addItems({"Pendiente", "En progreso", "Completada"});

    // El orden coincide con el enum Urgencia (Baja = 1, ...).
    comboUrgencia_->addItems({"Baja", "Media", "Alta"});

    editDescripcion_->setPlaceholderText("Por ejemplo: Estudiar para el examen");
    editDescripcion_->setMaxLength(200);

    editFechaLimite_->setCalendarPopup(true);
    editFechaLimite_->setDisplayFormat("yyyy/MM/dd HH:mm");
    editFechaLimite_->setDateTime(QDateTime::currentDateTime().addDays(1));

    auto* formulario = new QFormLayout;
    formulario->addRow("Tipo:", comboTipo_);
    formulario->addRow("Descripción:", editDescripcion_);
    formulario->addRow("Estado:", comboEstado_);
    formulario->addRow(etiquetaFecha_, editFechaLimite_);
    formulario->addRow(etiquetaUrgencia_, comboUrgencia_);

    auto* disenio = new QVBoxLayout(this);
    disenio->addLayout(formulario);
    disenio->addWidget(botones_);

    setMinimumWidth(380);
}

void DialogoTarea::actualizarCamposPorTipo() {
    const int tipo = comboTipo_->currentIndex();

    // Solo se muestran los campos del tipo elegido.
    const bool tieneFecha = (tipo == TipoConFecha);
    const bool tieneUrgencia = (tipo == TipoPrioritaria);

    etiquetaFecha_->setVisible(tieneFecha);
    editFechaLimite_->setVisible(tieneFecha);
    etiquetaUrgencia_->setVisible(tieneUrgencia);
    comboUrgencia_->setVisible(tieneUrgencia);

    adjustSize();
}

void DialogoTarea::cargar(const Tarea& tarea) {
    // Se usan solo metodos virtuales/comunes de Tarea:
    // tipo() dice que opcion elegir y datoExtra() trae el dato propio del tipo.
    const int tipo = comboTipo_->findText(QString::fromStdString(tarea.tipo()));
    comboTipo_->setCurrentIndex(tipo >= 0 ? tipo : TipoNormal);

    editDescripcion_->setText(QString::fromStdString(tarea.getDescription()));
    comboEstado_->setCurrentIndex(static_cast<int>(tarea.getEstado()) - 1);
    fechaCreacion_ = tarea.getCreationTimePoint();

    const string extra = tarea.datoExtra();
    if (!extra.empty()) {
        if (comboTipo_->currentIndex() == TipoConFecha) {
            editFechaLimite_->setDateTime(aQDateTime(Tarea::desdeSegundos(stoll(extra))));
        }
        else if (comboTipo_->currentIndex() == TipoPrioritaria) {
            comboUrgencia_->setCurrentIndex(stoi(extra) - 1);
        }
    }
}

unique_ptr<Tarea> DialogoTarea::crearTarea() const {
    // Los constructores validan la descripcion y lanzan ErrorValidacion.
    const string descripcion = editDescripcion_->text().trimmed().toStdString();
    unique_ptr<Tarea> tarea;

    switch (comboTipo_->currentIndex()) {
    case TipoConFecha: {
        const QDateTime limite = editFechaLimite_->dateTime();
        if (!limite.isValid()) {
            throw ErrorValidacion("La fecha límite no es válida.");
        }
        tarea = make_unique<TareaConFecha>(descripcion, fechaCreacion_, aTimePoint(limite));
        break;
    }
    case TipoPrioritaria:
        tarea = make_unique<TareaPrioritaria>(
            descripcion, fechaCreacion_,
            static_cast<Urgencia>(comboUrgencia_->currentIndex() + 1));
        break;
    default:
        tarea = make_unique<Tarea>(descripcion, fechaCreacion_);
        break;
    }

    tarea->setEstado(static_cast<Estado>(comboEstado_->currentIndex() + 1));
    return tarea;
}

void DialogoTarea::accept() {
    // Antes de cerrar, se intenta crear la tarea con los datos capturados.
    // Si algo esta mal (descripcion vacia, fecha invalida), se avisa y
    // el dialogo NO se cierra.
    try {
        crearTarea();
    }
    catch (const ErrorTareas& e) {
        QMessageBox::warning(this, QString::fromStdString(e.titulo()),
                             QString::fromStdString(e.what()));
        editDescripcion_->setFocus();
        return;
    }

    QDialog::accept();  // datos correctos: ahora si se cierra
}
