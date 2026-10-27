#include "Rule.h"

//constructor que copia los 4 datos dichos en el .h
Rule::Rule(int id,
const Proposition& antecedente1,const Proposition& antecedente2, const Proposition& consecuente): id(id), antecedente1(antecedente1), antecedente2(antecedente2),consecuente(consecuente) {}


// getters que devuelven una referencia a la proposicion de dentro sin copia ni modificarla
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
// se calcula el minimo aca (el Y difuso), la regla se cumple tanto como su parte mas debil
double Rule::calcularActivacion() const {
    return std::min(antecedente1.getGradoPertenencia(), antecedente2.getGradoPertenencia());
}

// forma la frase de la regla ej: "Si Descuento es Medio y Necesidad es Alta Entonces Compra es Comprar"
std::string Rule::toString() const {
    return "Si " + antecedente1.toString() + " y " + antecedente2.toString() + " Entonces " + consecuente.toString();
}

// escribe la regla con su id ej: "R1: Si ...", devuelve os para poder encadenar <<
std::ostream& operator<<(std::ostream& os, const Rule& regla) {
    os << "R" << regla.getId() << ": " << regla.toString();
    return os;
}
