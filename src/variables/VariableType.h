#ifndef VARIABLE_TYPE_H
#define VARIABLE_TYPE_H

#include "../utils/text.h"
#include <stdexcept>
#include <string>

/**
 * Una variable linguistica del sistema de inferencia primero se fuzzifica a partir de un valor y aparece en el antecedente de las reglas
 * aparece en el consecuente de las reglas y al fonal se desfuzzifica
 */

 enum class VariableType {
    ENTRADA,
    SALIDA
 };

// convierte el tipo a texto para mostrarlo solo en entrada y salida
inline std::string tipoATexto(VariableType tipo) {
    return (tipo==VariableType::ENTRADA) ? "Entrada" : "Salida"; 
}

/**
 * Convierte el texto leido del archivo en un VariableType
 * Si el texto no es entrada ni salida 
 */

inline VariableType tipoDesdeTexto(const std::string& texto) {
    if(iguales(texto,"ENTRADA")) return VariableType::ENTRADA;
    if(iguales(texto, "SALIDA")) return VariableType::SALIDA;
    throw std::invalid_argument("tipo de variable '" + texto + "' inválido");
}

#endif