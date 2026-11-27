#include "BaseDatos.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <memory>

#include "Excepciones.h"
#include "TareaConFecha.h"
#include "TareaPrioritaria.h"

using namespace std;

const QString BaseDatos::NOMBRE_CONEXION = "lista_tareas";

namespace {
    // Texto de error que da Qt, para agregarlo al mensaje de ErrorBaseDatos.
    string textoError(const QSqlError& error) {
        return error.text().toStdString();
    }

    // "Fabrica" de tareas: crea el objeto del tipo correcto a partir
    // de los datos de una fila. Regresa un unique_ptr<Tarea> que en
    // realidad puede apuntar a TareaConFecha o TareaPrioritaria.
    unique_ptr<Tarea> crearTarea(const string& tipo,
                                 const string& descripcion,
                                 const chrono::system_clock::time_point& creacion,
                                 const QVariant& fechaLimite,
                                 const QVariant& urgencia)
    {
        if (tipo == "Normal") {
            return make_unique<Tarea>(descripcion, creacion);
        }
        if (tipo == "Con fecha") {
            if (fechaLimite.isNull()) {
                throw ErrorBaseDatos("La tarea \"" + descripcion + "\" no tiene fecha limite guardada.");
            }
            return make_unique<TareaConFecha>(descripcion, creacion,
                                              Tarea::desdeSegundos(fechaLimite.toLongLong()));
        }
        if (tipo == "Prioritaria") {
            const int valor = urgencia.toInt();
            if (urgencia.isNull() || valor < 1 || valor > 3) {
                throw ErrorBaseDatos("La tarea \"" + descripcion + "\" tiene una urgencia invalida.");
            }
            return make_unique<TareaPrioritaria>(descripcion, creacion, static_cast<Urgencia>(valor));
        }
        throw ErrorBaseDatos("Tipo de tarea desconocido en la base de datos: \"" + tipo + "\".");
    }
}

BaseDatos::BaseDatos(const QString& archivo)
    : archivo_(archivo),
      conectada_(false)
{
}

BaseDatos::~BaseDatos() {
    if (!QSqlDatabase::contains(NOMBRE_CONEXION)) {
        return;
    }
    {
        // La variable db tiene que dejar de existir antes de removeDatabase(),
        // por eso va en su propio bloque.
        QSqlDatabase db = QSqlDatabase::database(NOMBRE_CONEXION, false);
        db.close();
    }
    QSqlDatabase::removeDatabase(NOMBRE_CONEXION);
}

void BaseDatos::conectar() {
    QSqlDatabase db = QSqlDatabase::contains(NOMBRE_CONEXION)
        ? QSqlDatabase::database(NOMBRE_CONEXION, false)
        : QSqlDatabase::addDatabase("QSQLITE", NOMBRE_CONEXION);

    if (!db.isValid()) {
        throw ErrorBaseDatos("No esta disponible el driver QSQLITE de Qt.");
    }

    db.setDatabaseName(archivo_);
    if (!db.open()) {
        throw ErrorBaseDatos("No se pudo abrir la base de datos \"" + archivo_.toStdString() +
                             "\": " + textoError(db.lastError()));
    }

    conectada_ = true;
    crearTabla();
}

bool BaseDatos::estaConectada() const {
    return conectada_;
}

const QString& BaseDatos::archivo() const {
    return archivo_;
}

void BaseDatos::validarConexion() const {
    if (!conectada_) {
        throw ErrorBaseDatos("La base de datos no esta conectada.");
    }
}

void BaseDatos::ejecutar(QSqlQuery& consulta, const string& que) {
    if (!consulta.exec()) {
        throw ErrorBaseDatos("No se pudo " + que + ": " + textoError(consulta.lastError()));
    }
}

void BaseDatos::crearTabla() {
    QSqlQuery consulta(QSqlDatabase::database(NOMBRE_CONEXION));
    consulta.prepare(
        "CREATE TABLE IF NOT EXISTS tareas ("
        "  id             INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  tipo           TEXT    NOT NULL,"
        "  descripcion    TEXT    NOT NULL,"
        "  estado         INTEGER NOT NULL,"
        "  fecha_creacion INTEGER NOT NULL,"
        "  fecha_limite   INTEGER,"
        "  urgencia       INTEGER"
        ")");
    ejecutar(consulta, "crear la tabla de tareas");
}

void BaseDatos::llenarValores(QSqlQuery& consulta, const Tarea& tarea) {
    const string tipo = tarea.tipo();

    consulta.bindValue(":tipo", QString::fromStdString(tipo));
    consulta.bindValue(":descripcion", QString::fromStdString(tarea.getDescription()));
    consulta.bindValue(":estado", static_cast<int>(tarea.getEstado()));
    consulta.bindValue(":fecha_creacion", Tarea::aSegundos(tarea.getCreationTimePoint()));

    // datoExtra() es virtual: la tarea con fecha regresa los segundos de su
    // fecha limite y la prioritaria su urgencia. La normal regresa "".
    // Un QVariant() vacio se guarda como NULL.
    const string extra = tarea.datoExtra();
    QVariant fechaLimite;
    QVariant urgencia;
    if (tipo == "Con fecha") {
        fechaLimite = stoll(extra);
    }
    else if (tipo == "Prioritaria") {
        urgencia = stoi(extra);
    }
    consulta.bindValue(":fecha_limite", fechaLimite);
    consulta.bindValue(":urgencia", urgencia);
}

int BaseDatos::guardar(const Tarea& tarea) {
    validarConexion();

    QSqlQuery consulta(QSqlDatabase::database(NOMBRE_CONEXION));
    consulta.prepare(
        "INSERT INTO tareas (tipo, descripcion, estado, fecha_creacion, fecha_limite, urgencia) "
        "VALUES (:tipo, :descripcion, :estado, :fecha_creacion, :fecha_limite, :urgencia)");
    llenarValores(consulta, tarea);
    ejecutar(consulta, "guardar la tarea \"" + tarea.getDescription() + "\"");

    return consulta.lastInsertId().toInt();
}

void BaseDatos::actualizar(const Tarea& tarea) {
    validarConexion();
    if (tarea.getId() <= 0) {
        throw ErrorBaseDatos("La tarea \"" + tarea.getDescription() +
                             "\" no esta guardada en la base de datos.");
    }

    QSqlQuery consulta(QSqlDatabase::database(NOMBRE_CONEXION));
    consulta.prepare(
        "UPDATE tareas SET tipo = :tipo, descripcion = :descripcion, estado = :estado, "
        "fecha_creacion = :fecha_creacion, fecha_limite = :fecha_limite, urgencia = :urgencia "
        "WHERE id = :id");
    llenarValores(consulta, tarea);
    consulta.bindValue(":id", tarea.getId());
    ejecutar(consulta, "actualizar la tarea \"" + tarea.getDescription() + "\"");

    if (consulta.numRowsAffected() == 0) {
        throw ErrorBaseDatos("No se encontro la tarea \"" + tarea.getDescription() +
                             "\" en la base de datos.");
    }
}

void BaseDatos::eliminar(int id) {
    validarConexion();

    QSqlQuery consulta(QSqlDatabase::database(NOMBRE_CONEXION));
    consulta.prepare("DELETE FROM tareas WHERE id = :id");
    consulta.bindValue(":id", id);
    ejecutar(consulta, "eliminar la tarea");
}

void BaseDatos::cargarTodas(ListaTareas& lista) {
    validarConexion();

    QSqlQuery consulta(QSqlDatabase::database(NOMBRE_CONEXION));
    consulta.prepare(
        "SELECT id, tipo, descripcion, estado, fecha_creacion, fecha_limite, urgencia "
        "FROM tareas ORDER BY id");
    ejecutar(consulta, "leer las tareas");

    lista.limpiar();
    while (consulta.next()) {
        const string tipo = consulta.value("tipo").toString().toStdString();
        const string descripcion = consulta.value("descripcion").toString().toStdString();
        const int estado = consulta.value("estado").toInt();

        if (estado < 1 || estado > 3) {
            throw ErrorBaseDatos("La tarea \"" + descripcion + "\" tiene un estado invalido.");
        }

        unique_ptr<Tarea> tarea = crearTarea(
            tipo, descripcion,
            Tarea::desdeSegundos(consulta.value("fecha_creacion").toLongLong()),
            consulta.value("fecha_limite"),
            consulta.value("urgencia"));

        tarea->setEstado(static_cast<Estado>(estado));
        tarea->setId(consulta.value("id").toInt());
        lista.agregarTarea(move(tarea));
    }
}
