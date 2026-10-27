#ifndef RULE_H
#define RULE_H

#include "Proposition.h"
#include <algorithm>
#include <ostream>
#include <string>

/**
 * Modela una regla difusa del tipo Mamdani con dos variables de entrada
 * y una variable de salida:
 * SI <Ant1> Y <Ant2> ENTONCES <Consec>
 */
class Rule {
    // atributos al crear una regla como se escribiria con dos antecedentes y consecuentes
    int id; // numero de la regla R1, R2...
    Proposition antecedente1; // primera condicion ej: Descuento es Medio
    Proposition antecedente2; // segunda condicion ej: Necesidad es Alta
    Proposition consecuente; // conclusion ej: Compra es Comprar

public:
    // constructor
    Rule(int id, const Proposition& antecedente1, const Proposition& antecedente2, const Proposition& consecuente);
    // getters
    int getId() const;
    const Proposition& getAntecedente1() const;
    const Proposition& getAntecedente2() const;
    const Proposition& getConsecuente() const;

    /**
     * Calcula la fuerza de disparo (grado de activación alpha)
     * usando la T-norma estándar del Mínimo:
     * alpha = min(mu_ant1, mu_ant2) usando el minimo de ambas
     */
    double calcularActivacion() const;

    /** Representación lingüística de la regla arma la regla en texto */
    std::string toString() const;
};

/** Escribe la regla con su id para poder hacer cout << regla, ej: "R1: Si ..." */
std::ostream& operator<<(std::ostream& os, const Rule& regla);

#endif
