#include "DiffuseSet.h"
#include "../utils/text.h"

#include <iomanip>
#include <stdexcept>

DiffuseSet::DiffuseSet(const std::string& nombre) :nombre(nombre), gradoPertenencia(0.0) {
    if (nombre.empty()) {
        throw std::invalid_argument("el conjunto debe tener un nombre");
    }
}

double DiffuseSet::fuzzificar(double x) {
    // se implementa el polimorfismo aca con el metodo al calcularlo y asignarlo mostrandolo de forma virtual en el conjunto
    gradoPertenencia = calcularPertenencia(x);
    return gradoPertenencia;
}

// se reinicia el grado para cada una de las iteraciones
void DiffuseSet::reiniciarGrado() {
    gradoPertenencia = 0.0;
}

const std::string& DiffuseSet::getNombre() const {
    return nombre;
}

double DiffuseSet::getGradoPertenencia() const {
    return gradoPertenencia;
}

std::string DiffuseSet::formatearPuntos(std::initializer_list<double> puntos) {
    std::string resultado = "(";
    bool primero = true;
    for(double p: puntos) {
        if(!primero) {
            resultado += ", ";
        }
        resultado+= texto::formatear(p);
        primero = false;
    }
    return resultado + ")";
}

std::ostream& operator<<(std::ostream& os, const DiffuseSet& conjunto) {
    os << std::left <<std::setw(12)<<conjunto.getNombre() << " "<< std::setw(12) << conjunto.getTipo() << " " << conjunto.getPuntosTexto();
    return os;
}