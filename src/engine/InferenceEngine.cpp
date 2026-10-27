#include "InferenceEngine.h"
#include <algorithm>
#include <iostream>

using namespace std;

InferenceEngine::InferenceEngine(const KnowledgeBase& base, LinguisticVariable& entrada1, LinguisticVariable& entrada2, const LinguisticVariable& salida) : 
    base(base), entrada1(entrada1), entrada2(entrada2), salida(salida) {}

vector<double> InferenceEngine::agregar() const {
    const auto& conjuntos = salida.getConjuntos();
    vector<double> alphas(conjuntos.size(), 0.0);   // arrancan en 0

    // Recorrer todas las reglas
    for (const Rule& regla : base.getReglas()) {
        double alpha = regla.calcularActivacion();
        const string& destino = regla.getConsecuente().getConjuntoNombre();

        // Buscar a cuál conjunto de salida apunta la regla y quedarse con el mayor alpha
        for (size_t j = 0; j < conjuntos.size(); j++) {
            if (conjuntos[j]->getNombre() == destino) {
                alphas[j] = max(alphas[j], alpha);
            }
        }
    }

    // Mostrar el resultado (traza)
    cout << " Agregacion ";
    for (size_t j = 0; j < conjuntos.size(); j++) {
        cout << "   " << salida.getNombre() << " ES " << conjuntos[j]->getNombre()
             << "  ->  alpha = " << alphas[j] << "\n";
    }
    return alphas;
}

double InferenceEngine::defuzzificar(const vector<double>& alphas) const {
    const auto& conjuntos = salida.getConjuntos();

    double inicio = salida.getMinimo();
    double fin = salida.getMaximo();
    int puntos = 1000;                       // cuántos trocitos se usan para recorrer el universo
    double paso = (fin - inicio) / puntos;

    double numerador = 0.0;                  // suma de x * mu
    double denominador = 0.0;                // suma de mu

    for (int i = 0; i <= puntos; i++) {
        double x = inicio + i * paso;

        // Altura de la figura agregada en x:
        // se recorta cada conjunto a su alpha (min) y se une con el resto (max)
        double mu = 0.0;
        for (size_t j = 0; j < conjuntos.size(); j++) {
            double recortado = min(alphas[j], conjuntos[j]->calcularPertenencia(x));
            mu = max(mu, recortado);
        }

        numerador += x * mu;
        denominador += mu;
    }

    cout << " Defuzzificacion";
    if (denominador == 0.0) {
        // Ninguna regla se activó: no hay figura que promediar
        cout << "   Ninguna regla se activo; se retorna el centro";
        return (inicio + fin) / 2.0;
    }

    double resultado = numerador / denominador;
    cout << "   Centroide = " << resultado << " " << salida.getUnidad() << "\n";
    return resultado;
}

double InferenceEngine::inferir(double valor1, double valor2) {
    cout << "\n Fuzzificacion \n";
    entrada1.fuzzificar(valor1);
    entrada2.fuzzificar(valor2);
    entrada1.mostrarFuzzificacion();
    entrada2.mostrarFuzzificacion();

    cout << " Evaluacion de reglas ";
    base.mostrarActivaciones();

    // Etapas 3 y 4 (ya imprimen su propia traza)
    vector<double> alphas = agregar();
    return defuzzificar(alphas);
}
