#include "InferenceEngine.h"
#include <algorithm>
#include <iostream>

using namespace std;

InferenceEngine::InferenceEngine(const KnowledgeBase& base, LinguisticVariable& entrada1, 
        LinguisticVariable& entrada2, const LinguisticVariable& salida) : 
    base(base), entrada1(entrada1), entrada2(entrada2), salida(salida) {}

vector<double> InferenceEngine::agregar() const {
    // Conjuntos de la variable de salida (NoComprar, Pensarlo, Comprar)
    const auto& conjuntos = salida.getConjuntos();

    // Un alpha por conjunto de salida. Todos arrancan en 0 (nadie activado)
    vector<double> alphas(conjuntos.size(), 0.0);

    // Recorrer todas las reglas de la base de conocimiento
    for (const Rule& regla : base.getReglas()) {
        // Fuerza de disparo de la regla: min(mu antecedente1, mu antecedente2)
        double alpha = regla.calcularActivacion();

        // Nombre del conjunto de salida que concluye esta regla
        const string& destino = regla.getConsecuente().getConjuntoNombre();

        // Buscar a cuál conjunto de salida apunta la regla y quedarse con el
        // mayor alpha (operador max entre reglas que concluyen lo mismo)
        for (size_t j = 0; j < conjuntos.size(); j++) {
            if (conjuntos[j]->getNombre() == destino) {
                alphas[j] = max(alphas[j], alpha);
            }
        }
    }

    // Traza: mostrar el alpha final de cada conjunto de salida
    cout << "\n   AGREGACION \n";
    for (size_t j = 0; j < conjuntos.size(); j++) {
        cout << "   " << salida.getNombre() << " ES " << conjuntos[j]->getNombre()
             << "  ->  alpha = " << alphas[j] << "\n";
    }
    return alphas;
}

double InferenceEngine::defuzzificar(const vector<double>& alphas) const {
    const auto& conjuntos = salida.getConjuntos();

    // Universo de la salida y tamaño del paso de muestreo
    double inicio = salida.getMinimo();
    double fin = salida.getMaximo();
    int puntos = 1000;                       // cuántos trocitos se usan para recorrer el universo
    double paso = (fin - inicio) / puntos;

    double numerador = 0.0;                  // suma de x * mu
    double denominador = 0.0;                // suma de mu

    // Recorrer el universo desde inicio hasta fin (incluyendo ambos extremos)
    for (int i = 0; i <= puntos; i++) {
        double x = inicio + i * paso;

        // Altura de la figura agregada en x:
        // se recorta cada conjunto a su alpha (min) y se une con el resto (max)
        double mu = 0.0;
        for (size_t j = 0; j < conjuntos.size(); j++) {
            // calcularPertenencia evalúa mu(x) sin modificar el conjunto
            double recortado = min(alphas[j], conjuntos[j]->calcularPertenencia(x));
            mu = max(mu, recortado);
        }

        // Acumular para el promedio ponderado
        numerador += x * mu;
        denominador += mu;
    }

    cout << "\n   DEFUZZIFICACION (centroide)\n";
    if (denominador == 0.0) {
        // Ninguna regla se activó: no hay figura que promediar (evita dividir por 0).
        // Se retorna el centro del universo como valor por defecto.
        cout << "   Ninguna regla se activo; se retorna el centro del universo.\n";
        return (inicio + fin) / 2.0;
    }

    double resultado = numerador / denominador;
    cout << "   Centroide = " << resultado << " " << salida.getUnidad() << "\n";
    return resultado;
}

double InferenceEngine::inferir(double valor1, double valor2) {
    // Etapa 1: Fuzzificación. Cada conjunto de cada variable de entrada
    // guarda internamente su grado de pertenencia para el valor dado.
    cout << "\n   FUZZIFICACION \n";
    entrada1.fuzzificar(valor1);
    entrada2.fuzzificar(valor2);
    entrada1.mostrarFuzzificacion();
    entrada2.mostrarFuzzificacion();

    // Etapa 2: Evaluación de reglas. Las proposiciones de las reglas leen los
    // grados guardados en el paso anterior y calculan alpha = min(mu1, mu2).
    cout << "\n   EVALUACION DE REGLAS \n";
    base.mostrarActivaciones();

    // Etapas 3 y 4 
    vector<double> alphas = agregar();
    return defuzzificar(alphas);
}
