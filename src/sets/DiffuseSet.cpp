#include "DiffuseSet.h"
#include "../utils/text.h"

#include <iomanip>
#include <stdexcept>

// el constructor guarda el nombre y empieza con un grado de perenencia 0
DiffuseSet::DiffuseSet(const std::string& nombre) :nombre(nombre), gradoPertenencia(0.0) {
    if (nombre.empty()) {
        throw std::invalid_argument("el conjunto debe tener un nombre");
    }
}

double DiffuseSet::fuzzificar(double x) {
    // se implementa el polimorfismo aca con el metodo al calcularlo y se llama al triangulo o trapecio segun aplique y se guarda su grado.
    gradoPertenencia = calcularPertenencia(x);
    return gradoPertenencia;
}

// se reinicia el grado para cada una de las iteraciones poniendolo en 0
void DiffuseSet::reiniciarGrado() {
    gradoPertenencia = 0.0;
}

const std::string& DiffuseSet::getNombre() const {
    return nombre;
}

double DiffuseSet::getGradoPertenencia() const {
    return gradoPertenencia;
}
// va formateando los numeros a dobles.
std::string DiffuseSet::formatearPuntos(std::initializer_list<double> puntos) {
    std::string resultado = "(";
    bool primero = true;
    for(double p: puntos) {
        if(!primero) {
            resultado += ", ";
        }
        resultado+= formatear(p);
        primero = false;
    }
    return resultado + ")";
}
// encadena los puntos con las variables y conjunto respectivos
std::ostream& operator<<(std::ostream& os, const DiffuseSet& conjunto) {
    os << std::left <<std::setw(12)<<conjunto.getNombre() << " "<< std::setw(12) << conjunto.getTipo() << " " << conjunto.getPuntosTexto();
    return os;
}