#include "Rule.h"

Rule::Rule(int id,
           const Proposition& antecedente1,
           const Proposition& antecedente2,
           const Proposition& consecuente)
    : id(id),
      antecedente1(antecedente1),
      antecedente2(antecedente2),
      consecuente(consecuente) {}

int Rule::getId() const {
    return id;
}

const Proposition& Rule::getAntecedente1() const {
    return antecedente1;
}

const Proposition& Rule::getAntecedente2() const {
    return antecedente2;
}

const Proposition& Rule::getConsecuente() const {
    return consecuente;
}

double Rule::calcularActivacion() const {
    return std::min(antecedente1.getGradoPertenencia(), antecedente2.getGradoPertenencia());
}

std::string Rule::toString() const {
    return "SI " + antecedente1.toString() +
           " Y " + antecedente2.toString() +
           " ENTONCES " + consecuente.toString();
}

std::ostream& operator<<(std::ostream& os, const Rule& regla) {
    os << "R" << regla.getId() << ": " << regla.toString();
    return os;
}
