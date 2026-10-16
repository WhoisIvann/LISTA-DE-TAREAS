#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <locale>
#include <sstream>
#include <string>

using namespace std;

int leerEntero(const string& prompt, int minVal, int maxVal) {  // Se utiliza para leer un entero dentro de un rango específico
    while (true) {
        cout << prompt;

        int valor;

        if (cin >> valor) {
            if (valor >= minVal and valor <= maxVal) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return valor;
            }
        }

        cout << "Entrada no valida. Introduzca un numero entre "
            << minVal
            << " y "
            << maxVal
            << ".\n";

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string leerLineaNoVacia(const string& prompt) { // Se utiliza para leer una línea de texto que no esté vacía
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

chrono::system_clock::time_point leerFecha(const string& prompt) { // Se utiliza para leer una fecha y hora
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

            tmStruct.tm_hour = 0;
            tmStruct.tm_min = 0;
            tmStruct.tm_sec = 0;
        }
        else {
            iss >> get_time(&tmStruct, "%Y/%m/%d %H:%M:%S");
        }

        if (iss.fail()) {
            cout << "Formato invalido. Use YYYY/MM/DD o "
                << "YYYY/MM/DD HH:MM:SS\n";
            continue;
        }

        time_t tt = mktime(&tmStruct);

        if (tt == -1) {
            cout << "Fecha invalida. Intente de nuevo.\n";
            continue;
        }

        return chrono::system_clock::from_time_t(tt);
    }
}

void mostrarMenu() {
    cout << "___ Administrador de Tareas ___\n";
    cout << "1. Agregar tarea\n";
    cout << "2. Listar tareas\n";
    cout << "3. Cambiar estado de una tarea\n";
    cout << "4. Eliminar tarea\n";
    cout << "0. Salir\n";
}

#endif