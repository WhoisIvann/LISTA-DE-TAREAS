#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <chrono>
#include <string>

// Lee un entero dentro de un rango especifico.
int leerEntero(const std::string& prompt, int minVal, int maxVal);

// Lee una linea de texto que no este vacia.
std::string leerLineaNoVacia(const std::string& prompt);

// Lee una fecha con formato YYYY/MM/DD o YYYY/MM/DD HH:MM:SS.
std::chrono::system_clock::time_point leerFecha(const std::string& prompt);

void mostrarMenu();

#endif
