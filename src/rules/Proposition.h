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
    Proposition() : conjuntoPtr(nullptr), variablePtr(nullptr) {}
    Proposition(const std::string& variableNombre, const std::string& conjuntoNombre, const DiffuseSet* conjuntoPtr = nullptr, const LinguisticVariable* variablePtr = nullptr) :variableNombre(variableNombre), conjuntoNombre(conjuntoNombre), conjuntoPtr(conjuntoPtr), variablePtr(variablePtr) {}

    const std::string& getVariableNombre() const {
         return variableNombre; 
        }
    const std::string& getConjuntoNombre() const { return conjuntoNombre; }
    const DiffuseSet* getConjunto() const { return conjuntoPtr; }
    const LinguisticVariable* getVariable() const { return variablePtr; }

    void setConjunto(const DiffuseSet* ptr) { conjuntoPtr = ptr; }
    void setVariable(const LinguisticVariable* ptr) { variablePtr = ptr; }

    /** Retorna el grado de pertenencia registrado actualmente en el conjunto difuso */
    double getGradoPertenencia() const {
        return conjuntoPtr ? conjuntoPtr->getGradoPertenencia() : 0.0;
    }

    std::string toString() const {
        return variableNombre + " es " + conjuntoNombre;
    }

private:
    std::string variableNombre;
    std::string conjuntoNombre;
    const DiffuseSet* conjuntoPtr;
    const LinguisticVariable* variablePtr;
};

#endif
