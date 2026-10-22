#include "Utilidades.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

using namespace std;

int leerEntero(const string& prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;

        int valor;

        if (cin >> valor) {
            if (valor >= minVal && valor <= maxVal) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return valor;
            }
        }

        cout << "Entrada no valida. Introduzca un numero entre "
             << minVal << " y " << maxVal << ".\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string leerLineaNoVacia(const string& prompt) {
    string line;

    while (true) {
        cout << prompt;
        getline(cin, line);

        if (!line.empty()) {
            return line;
        }

        cout << "La entrada no puede estar vacia. Intente de nuevo.\n";
    }
}

chrono::system_clock::time_point leerFecha(const string& prompt) {
    while (true) {
        cout << prompt;

        string line;
        getline(cin, line);

        if (line.empty()) {
            cout << "La fecha no puede estar vacia.\n";
            continue;
        }

        tm tmStruct = {};
        istringstream iss(line);

        if (line.size() == 10) {
            iss >> get_time(&tmStruct, "%Y/%m/%d");
            // Solo fecha: se toma el final del dia como limite
            tmStruct.tm_hour = 23;
            tmStruct.tm_min = 59;
            tmStruct.tm_sec = 0;
        }
        else {
            iss >> get_time(&tmStruct, "%Y/%m/%d %H:%M:%S");
        }

        if (iss.fail()) {
            cout << "Formato invalido. Use YYYY/MM/DD o YYYY/MM/DD HH:MM:SS\n";
            continue;
        }

        tmStruct.tm_isdst = -1;
        time_t tt = mktime(&tmStruct);

        if (tt == -1) {
            cout << "Fecha invalida. Intente de nuevo.\n";
            continue;
        }

        return chrono::system_clock::from_time_t(tt);
    }
}

void mostrarMenu() {
    cout << "___ Administrador de Tareas (Fase 2) ___\n";
    cout << "1. Agregar tarea normal\n";
    cout << "2. Agregar tarea con fecha limite\n";
    cout << "3. Agregar tarea prioritaria\n";
    cout << "4. Listar tareas\n";
    cout << "5. Ordenar por prioridad y fecha\n";
    cout << "6. Cambiar estado de una tarea\n";
    cout << "7. Eliminar tarea\n";
    cout << "0. Salir\n";
}
