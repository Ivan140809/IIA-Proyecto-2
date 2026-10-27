#ifndef RULE_H
#define RULE_H

#include "Proposition.h"
#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

/**
 * Modela una regla difusa del tipo Mamdani con variables de entrada (2, 3 o N)
 * y una variable de salida:
 * SI <Ant1> Y <Ant2> [Y <Ant3>] ENTONCES <Consec>
 */
class Rule {
    int id; // numero de la regla R1, R2...
    std::vector<Proposition> antecedentes; // condiciones ej: Descuento es Medio, etc.
    Proposition consecuente; // conclusion ej: Compra es Comprar

public:
    // constructores
    Rule(int id, const std::vector<Proposition>& antecedentes, const Proposition& consecuente);
    Rule(int id, const Proposition& antecedente1, const Proposition& antecedente2, const Proposition& consecuente);
    Rule(int id, const Proposition& antecedente1, const Proposition& antecedente2, const Proposition& antecedente3, const Proposition& consecuente);

    // getters
    int getId() const;
    const std::vector<Proposition>& getAntecedentes() const;
    const Proposition& getAntecedente(std::size_t index) const;
    const Proposition& getAntecedente1() const;
    const Proposition& getAntecedente2() const;
    const Proposition& getAntecedente3() const;
    const Proposition& getConsecuente() const;

    /**
     * Calcula la fuerza de disparo (grado de activación alpha)
     * usando la T-norma estándar del Mínimo para todos los antecedentes:
     * alpha = min(mu_ant1, mu_ant2, [mu_ant3, ...])
     */
    double calcularActivacion() const;

    /** Representación lingüística de la regla arma la regla en texto */
    std::string toString() const;
};

/** Escribe la regla con su id para poder hacer cout << regla, ej: "R1: Si ..." */
std::ostream& operator<<(std::ostream& os, const Rule& regla);

#endif
