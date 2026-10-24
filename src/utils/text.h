#ifndef TEXT_H
#define TEXT_H

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

/**
 * Funciones de texto usadas en todo el proyecto para la fase de variables linguisticas Difusas
 * Que provienen directamente de los archivos de texto de variables con la normalizacion para cada
 * variable que ingrese por el txt.
 */

namespace texto {

/** Devuelve una copia de s en mayusculas*/
inline std::string aMayus(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),[] (unsigned char c) {return static_cast<char>(std::toupper(c)); });
    return s;
}

/** Funcion de comparacion de cadenas */
inline bool iguales(const std::string& a, const std::string& b) {
    return aMayus(a) == aMayus(b);
}

/** Convierte un numero a texto con 2 decimales para la variable difusa */
inline std::string formatear(double valor, int decimales=2){
    std::ostringstream os;
    os <<std::fixed<<std::setprecision(decimales) <<valor;
    return os.str();
}

/** Convertir un texto a double con la verficacion si el texto no es un numero valido  */
inline double aNumero(const std::string& s) {
    try{
        std::size_t usados =0;
        double valor = std::stod(s, &usados);
        if (usados == s.size()) {
            return valor;
        }
    } catch(const std::exception&) {

    }
    throw std::invalid_argument( s + "no es un numero valido");
}
}

#endif