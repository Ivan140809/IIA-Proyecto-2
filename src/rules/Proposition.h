#ifndef PROPOSITION_H
#define PROPOSITION_H

#include "../sets/DiffuseSet.h"
#include "../variables/LinguisticVariable.h"
#include <string>

/**
 * Modela una proposición lingüística difusa elemental de la forma:
 * <Variable> ES <Conjunto>
 * Por ejemplo: "Descuento ES Medio"
 */
class Proposition {
public:
    // constructor vacio poniendo dos punteros de conjunto y variable en null
    Proposition() : conjuntoPtr(nullptr), variablePtr(nullptr) {}

    // constructor principal con los nombres y los punteros al conjunto y a la variable reales, si no se pasan quedan en null
    Proposition(const std::string& variableNombre, const std::string& conjuntoNombre, const DiffuseSet* conjuntoPtr = nullptr, const LinguisticVariable* variablePtr = nullptr) :variableNombre(variableNombre), conjuntoNombre(conjuntoNombre), conjuntoPtr(conjuntoPtr), variablePtr(variablePtr) {}

    // getters de variable y conjunto
    const std::string& getVariableNombre() const {
         return variableNombre; 
        }
    const std::string& getConjuntoNombre() const { return conjuntoNombre; }
    const DiffuseSet* getConjunto() const { return conjuntoPtr; }
    const LinguisticVariable* getVariable() const { return variablePtr; }

    //setters
    void setConjunto(const DiffuseSet* ptr) { conjuntoPtr = ptr; }
    void setVariable(const LinguisticVariable* ptr) { variablePtr = ptr; }

    /** Retorna el grado de pertenencia registrado actualmente en el conjunto difuso */
    double getGradoPertenencia() const {
        return conjuntoPtr ? conjuntoPtr->getGradoPertenencia() : 0.0;
    }

    // arma la proposicion en texto por ejemplo "Descuento es Medio"
    std::string toString() const {
        return variableNombre + " es " + conjuntoNombre;
    }

private:
// punteros normales no inteligentes la proposicion no es como tal que contenga al conjunto
    std::string variableNombre; // nombre de la variable ej: Descuento
    std::string conjuntoNombre; // nombre del conjunto ej: Medio
    const DiffuseSet* conjuntoPtr; // conjunto real del que se lee el grado de pertenencia
    const LinguisticVariable* variablePtr; // variable a la que pertenece el conjunto
};

#endif
