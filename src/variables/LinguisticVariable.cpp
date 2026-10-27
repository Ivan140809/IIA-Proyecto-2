#include "LinguisticVariable.h"
#include "../utils/text.h"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

// El valor nítido arranca en NaN no es un numero para indicar que la
// variable todavía no ha sido fuzzificada

LinguisticVariable::LinguisticVariable(const std::string& nombre, VariableType tipo, double minimo, double maximo, const std::string& unidad) :nombre(nombre), tipo(tipo), minimo(minimo), maximo(maximo), unidad(unidad), valorNitido(std::numeric_limits<double>::quiet_NaN()) {
    //verificacion de intervalo valido
    if(minimo >= maximo) {
        throw std::invalid_argument("variable '" + nombre + "': el mínimo (" + formatear(minimo) + ") debe ser menor que el máximo (" + formatear(maximo) + ")");
    }
}

void LinguisticVariable::agregarConjunto(std::unique_ptr<DiffuseSet> conjunto) {
    // no se permiten dos conjuntos con la misma etiqueta en una variable por ejemplo el conjunto bajo 
    if (getConjunto(conjunto->getNombre()) != nullptr) {
        throw std::invalid_argument("la variable '" + nombre + "' ya tiene un conjunto llamado '" + conjunto->getNombre() + "'");
    }
    // el soporte del conjunto debe quedar dentro del universo de valores definidos 
    if (conjunto->getLimiteInferior() < minimo || conjunto->getLimiteSuperior() > maximo) {
        throw std::invalid_argument("el conjunto '" + conjunto->getNombre() + "' se sale del universo [" + formatear(minimo) + ", " + formatear(maximo) + "] de la variable '" + nombre + "'");
    }
    // std::move transfiere la propiedad del conjunto al vector
    conjuntos.push_back(std::move(conjunto));
}

DiffuseSet* LinguisticVariable::getConjunto(const std::string& nombreConjunto) const {
    for (const auto& c:conjuntos) {
        if (iguales(c->getNombre(), nombreConjunto)) {
            return c.get(); // devuelve el puntero no el valor
        }
    }
    return nullptr;
}

void LinguisticVariable::fuzzificar(double valor) {
    // si el valor se sale del universo se satura al limite mas cercano [minimo, maximo]
    double x = std::max(minimo, std::min(maximo, valor));
    if (x != valor) {
        std::cout << "el valor " << formatear(valor) << " está fuera de [" << formatear(minimo) << ", " << formatear(maximo) << "], se usara " << formatear(x) << "\n";
    }
    valorNitido = x;
    // cada conjunto calcula y guarda internamente su propio grado de pertenencia
    for (auto& c : conjuntos) {
        c->fuzzificar(x); // se llama al fuzzificar del conjunto
    }
}

void LinguisticVariable::reiniciar() { // se vuelve a poner NaN y los grados en 0
    valorNitido = std::numeric_limits<double>::quiet_NaN();
    for (auto& c : conjuntos) {
        c->reiniciarGrado();
    }
}

void LinguisticVariable::mostrar() const {
    // datos generales de la variable
    std::cout << "\n" << " Variable linguistica: " << nombre << "  (" << tipoATexto(tipo) << ")\n" << " Universo : [" << formatear(minimo) << " , " << formatear(maximo) << "] " << unidad << "\n" << "Numero de conjuntos  : " << conjuntos.size() << "\n" << "\n" << "   " << std::left << std::setw(12) << "Conjunto" << " " << std::setw(12) << "Forma" << " Puntos\n";
    // una linea por conjunto, usando el operator<< de DiffuseSet
    for (const auto& c : conjuntos) {
        std::cout << "   " << *c << "\n";
    }
    std::cout << "\n";
}

void LinguisticVariable::mostrarFuzzificacion() const {
    if (std::isnan(valorNitido)) {
        std::cout << "  La variable '" << nombre << "' aun no ha sido fuzzificada.\n";
        return;
    }
    std::cout << "\n Fuzzificacion de " << nombre << " con x = " << formatear(valorNitido) << " " << unidad << "\n";
    // grado de pertenencia que quedo registrado en cada conjunto
    for (const auto& c : conjuntos) {
        std::cout << "μ" << std::left << std::setw(12) << c->getNombre() << " = " << formatear(c->getGradoPertenencia(), 4) << "\n";
    }
}