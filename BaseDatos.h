#ifndef BASE_DATOS_H
#define BASE_DATOS_H

#include <QString>
#include <string>

#include "ListaTareas.h"
#include "Tarea.h"

class QSqlQuery;

// Base de datos de la aplicacion (Fase 3): guarda las tareas en un
// archivo SQLite (tareas.db) usando QtSql, para que no se pierdan
// al cerrar la app.
//
// Tabla "tareas":
//   id              INTEGER  llave primaria, autoincrement
//   tipo            TEXT     "Normal", "Con fecha" o "Prioritaria" (tipo())
//   descripcion     TEXT
//   estado          INTEGER  1 = Pendiente, 2 = En progreso, 3 = Completada
//   fecha_creacion  INTEGER  segundos desde 1970
//   fecha_limite    INTEGER  segundos desde 1970 (solo "Con fecha", si no NULL)
//   urgencia        INTEGER  1 = Baja, 2 = Media, 3 = Alta (solo "Prioritaria", si no NULL)
//
// Todas las consultas usan prepare() + bindValue(), nunca se arma el SQL
// pegando textos (asi una descripcion con comillas no rompe nada).
// Si algo falla se lanza ErrorBaseDatos (Excepciones.h) con el texto
// de lastError(), y la ventana se encarga de avisarle al usuario.
class BaseDatos {
private:
    QString archivo_;
    bool conectada_;

    // Nombre de la conexion dentro de Qt.
    static const QString NOMBRE_CONEXION;

    void crearTabla();

    // Ejecuta la consulta ya preparada; si falla lanza ErrorBaseDatos.
    // "que" describe la operacion para el mensaje de error ("guardar la tarea").
    void ejecutar(QSqlQuery& consulta, const std::string& que);

    // Lanza ErrorBaseDatos si todavia no se llamo a conectar().
    void validarConexion() const;

    // Pone en la consulta los valores de la tarea (:tipo, :descripcion, ...).
    static void llenarValores(QSqlQuery& consulta, const Tarea& tarea);

public:
    // archivo: ruta del archivo .db (se crea si no existe).
    explicit BaseDatos(const QString& archivo = "tareas.db");
    ~BaseDatos();

    // No se puede copiar: solo debe haber una conexion abierta.
    BaseDatos(const BaseDatos&) = delete;
    BaseDatos& operator=(const BaseDatos&) = delete;

    // Abre (o crea) el archivo y crea la tabla si no existe.
    // Lanza ErrorBaseDatos si no se puede abrir.
    void conectar();
    bool estaConectada() const;

    const QString& archivo() const;

    // Inserta la tarea y regresa el id que le dio la base de datos.
    int guardar(const Tarea& tarea);

    // Actualiza la fila de la tarea (se busca por getId()): descripcion,
    // estado y tambien tipo y datos extra, porque al editar se puede
    // cambiar el tipo. Lanza ErrorBaseDatos si la tarea no esta guardada.
    void actualizar(const Tarea& tarea);

    // Borra la fila con ese id.
    void eliminar(int id);

    // Borra lo que tenga la lista y la llena con todas las tareas guardadas,
    // creando el objeto correcto segun la columna tipo.
    void cargarTodas(ListaTareas& lista);
};

#endif
