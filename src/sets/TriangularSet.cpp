#include "TriangularSet.h"
#include <string>
#include <stdexcept>

// herencia del conjunto difuso recibiendo el nombre y los puntos a, b y c, llama primero al constructor de la base y despues guarda a,b,c
TriangularSet::TriangularSet(const std::string& nombre, double a, double b, double c): DiffuseSet(nombre), a(a), b(b), c(c) {
    if(!(a <= b && b<= c) || a==c) {
        //error si los puntos no estan en orden o si el triangulo no tiene anchura
        throw std::invalid_argument("Triangular '" + nombre + "': se requiere a <= b <= c y a < c, se recibió " + formatearPuntos({a,b,c}));
    }
}

// pertenencia segun el pico del triangulo y su posicion
double TriangularSet::calcularPertenencia(double x) const {
    if (x==b){
        return 1.0; // traduccion a pico
    }
    if(x<=a || x>= c) {
        return 0.0; //traduccion de pico y lados horizontales fuera del triangulo
    }
    if(x < b ) {
        return (x-a) / (b-a); //rampa de subida
    }else {
        return (c-x) / (c -b); //rampa de bajada
    }
}

// getters
std::string TriangularSet::getTipo() const {
    return "Triangular";
}

// getters de puntos
std::string TriangularSet::getPuntosTexto() const {
    return formatearPuntos({a,b,c});
}

// limite a 
double TriangularSet::getLimiteInferior() const {
    return a;
}
// limite c
double TriangularSet::getLimiteSuperior() const {
    return c;
}