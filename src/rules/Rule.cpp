#include "Rule.h"
#include <stdexcept>

// constructor principal a partir de un vector de antecedentes
Rule::Rule(int id, const std::vector<Proposition>& antecedentes, const Proposition& consecuente)
    : id(id), antecedentes(antecedentes), consecuente(consecuente) {}

// constructor para 2 antecedentes (compatibilidad)
Rule::Rule(int id, const Proposition& antecedente1, const Proposition& antecedente2, const Proposition& consecuente)
    : id(id), antecedentes{antecedente1, antecedente2}, consecuente(consecuente) {}

// constructor para 3 antecedentes
Rule::Rule(int id, const Proposition& antecedente1, const Proposition& antecedente2, const Proposition& antecedente3, const Proposition& consecuente)
    : id(id), antecedentes{antecedente1, antecedente2, antecedente3}, consecuente(consecuente) {}

// getters que devuelven una referencia a la proposicion de dentro sin copia ni modificarla
int Rule::getId() const {
    return id;
}

const std::vector<Proposition>& Rule::getAntecedentes() const {
    return antecedentes;
}

const Proposition& Rule::getAntecedente(std::size_t index) const {
    if (index >= antecedentes.size()) {
        throw std::out_of_range("Índice de antecedente fuera de rango.");
    }
    return antecedentes[index];
}

const Proposition& Rule::getAntecedente1() const {
    return getAntecedente(0);
}

const Proposition& Rule::getAntecedente2() const {
    return getAntecedente(1);
}

const Proposition& Rule::getAntecedente3() const {
    return getAntecedente(2);
}

const Proposition& Rule::getConsecuente() const {
    return consecuente;
}

// se calcula el minimo aca (el Y difuso), la regla se cumple tanto como su parte mas debil
double Rule::calcularActivacion() const {
    if (antecedentes.empty()) {
        return 0.0;
    }
    double minAlpha = antecedentes[0].getGradoPertenencia();
    for (std::size_t i = 1; i < antecedentes.size(); ++i) {
        minAlpha = std::min(minAlpha, antecedentes[i].getGradoPertenencia());
    }
    return minAlpha;
}

// forma la frase de la regla ej: "Si Descuento es Medio y Necesidad es Alta y Clima es Lluvioso Entonces Compra es Comprar"
std::string Rule::toString() const {
    if (antecedentes.empty()) {
        return "Entonces " + consecuente.toString();
    }
    std::string res = "Si " + antecedentes[0].toString();
    for (std::size_t i = 1; i < antecedentes.size(); ++i) {
        res += " y " + antecedentes[i].toString();
    }
    res += " Entonces " + consecuente.toString();
    return res;
}

// escribe la regla con su id ej: "R1: Si ...", devuelve os para poder encadenar <<
std::ostream& operator<<(std::ostream& os, const Rule& regla) {
    os << "R" << regla.getId() << ": " << regla.toString();
    return os;
}
