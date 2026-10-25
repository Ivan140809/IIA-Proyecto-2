#include "TriangularSet.h"
#include <string>
#include <stdexcept>

TriangularSet::TriangularSet(const std::string& nombre, double a, double b, double c): DiffuseSet(nombre), a(a), b(b), c(c) {
    if(!(a <= b && b<= c) || a==c) {
        throw std::invalid_argument("Triangular '" + nombre + "': se requiere a <= b <= c y a < c, se recibió " + formatearPuntos({a,b,c}));
    }
}

double TriangularSet::calcularPertenencia(double x) const {
    if (x==b){
        return 1.0; // traduccion a pico
    }
    if(x<=a || x>= c) {
        return 0.0; //traduccion de pico y lados verticales
    }
    if(x < b ) {
        return (x-a) / (b-a); //rampa de subida
    }else {
        return (c-x) / (c -b); //rampa de bajada
    }
}

std::string TriangularSet::getTipo() const {
    return "Triangular";
}

std::string TriangularSet::getPuntosTexto() const {
    return formatearPuntos({a,b,c});
}

double TriangularSet::getLimiteInferior() const {
    return a;
}

double TriangularSet::getLimiteSuperior() const {
    return c;
}