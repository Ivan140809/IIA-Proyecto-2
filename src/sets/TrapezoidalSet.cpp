#include "TrapezoidalSet.h"

#include <stdexcept>

TrapezoidalSet::TrapezoidalSet(const std::string& nombre, double a, double b, double c, double d) : DiffuseSet(nombre), a(a), b(b), c(c), d(d) {
    if(!(a <= b && b<=c && c<=d)) {
        throw std::invalid_argument("trapezoidal '" + nombre + "': se requiere a <= b <= c <= d y a < d, se recibió " + formatearPuntos({a, b, c, d}));
    }
}

double TrapezoidalSet::calcularPertenencia(double x) const {
    if(x >= b && x<=c){
        return 1.0; //meseta 
    }
    if(x<= a || x>= d) {
        return 0.0; // fuera del trapecio
    }
    if(x<b){
        return (x-a) / (b-a); //rampa de subida
    }
    return (d-x) / (d-c); //rampa de bajada
}

std::string TrapezoidalSet::getTipo() const {
    return "Trapezoidal";
}

std::string TrapezoidalSet::getPuntosTexto() const {
    return formatearPuntos({a,b,c,d});
}

double TrapezoidalSet::getLimiteInferior() const {
    return a;
}

double TrapezoidalSet::getLimiteSuperior() const {
    return d;
}