#ifndef INFERENCE_ENGINE_H
#define INFERENCE_ENGINE_H

#include "../rules/KnowledgeBase.h"
#include "../variables/LinguisticVariable.h"
#include <vector>

/**
 * Motor de inferencia difusa tipo Mamdani con dos variables de entrada y una de salida.
 * Ejecuta las 4 etapas: fuzzificacion, evaluacion de reglas, agregacion y defuzzificacion.
 *
 * Trabaja con referencias a la base de conocimiento y a las variables, no hace copias.
 */
class InferenceEngine {
    private:
        const KnowledgeBase& base; // reglas que se evaluan
        std::vector<LinguisticVariable*> entradas; // const porque se fuzzifica
        const LinguisticVariable& salida; // solo se lee para agregar y defuzzificar

    public:
        /** Recibe la base y las 3 variables, entrada1 y entrada2 deben ir en el mismo orden que los valores de inferir() */
        InferenceEngine(const KnowledgeBase& base, const std::vector<LinguisticVariable*>& entradas, const LinguisticVariable& salida);

        /**
        * Etapa 3: para cada conjunto de la variable de salida, calcula el
        * máximo alpha entre las reglas que lo concluyen.
        * Retorna un valor por conjunto, en el mismo orden de la variable de salida.
        */
        std::vector<double> agregar() const;

        /**
        * Etapa 4: calcula el centroide de la salida agregada.
        * Recibe los alphas que devolvió agregar().
        */
        double defuzzificar(const std::vector<double>& alphas) const;

        /**
        * Ejecuta las 4 etapas del proceso Mamdani para un par de valores
        * de entrada y retorna el valor de la salida.
        */
        double inferir(const std::vector<double>& valores);
};

#endif
