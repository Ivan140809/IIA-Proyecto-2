#ifndef INFERENCE_ENGINE_H
#define INFERENCE_ENGINE_H

#include "../rules/KnowledgeBase.h"
#include "../variables/LinguisticVariable.h"
#include <vector>

/**
 * Motor de inferencia difusa tipo Mamdani.
 * Por ahora solo implementa la etapa de agregación.
 */
class InferenceEngine {
public:
    InferenceEngine(const KnowledgeBase& base, LinguisticVariable& entrada1,
                    LinguisticVariable& entrada2, const LinguisticVariable& salida);

    /**
     * Etapa 3: para cada conjunto de la variable de salida, calcula el
     * máximo alpha entre las reglas que lo concluyen.
     * Retorna un valor por conjunto, en el mismo orden de la variable de salida.
     */
    std::vector<double> agregar() const;

    /**
     * Etapa 4: calcula el centroide de la salida agregada.
     * Recibe los alphas que devolvió agregar() y retorna el valor nítido.
     */
    double defuzzificar(const std::vector<double>& alphas) const;

    /**
     * Ejecuta las 4 etapas del proceso Mamdani para un par de valores
     * de entrada y retorna el valor nítido de la salida.
     */
    double inferir(double valor1, double valor2);


private:
    const KnowledgeBase& base;
    LinguisticVariable& entrada1;
    LinguisticVariable& entrada2;
    const LinguisticVariable& salida;
};

#endif
