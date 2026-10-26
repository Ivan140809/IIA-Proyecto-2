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
    int id;
    Proposition antecedente1;
    Proposition antecedente2;
    Proposition consecuente;

public:
    Rule(int id, const Proposition& antecedente1, const Proposition& antecedente2, const Proposition& consecuente);

    int getId() const;
    const Proposition& getAntecedente1() const;
    const Proposition& getAntecedente2() const;
    const Proposition& getConsecuente() const;

    /**
     * Calcula la fuerza de disparo (grado de activación alpha)
     * usando la T-norma estándar del Mínimo:
     * alpha = min(mu_ant1, mu_ant2)
     */
    double calcularActivacion() const;

    /** Representación lingüística de la regla */
    std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const Rule& regla);

#endif
