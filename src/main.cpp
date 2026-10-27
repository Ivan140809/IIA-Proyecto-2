#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include "variables/VariableCharger.h"
#include "rules/KnowledgeBase.h"
#include "engine/InferenceEngine.h"

// corre las fases 1 a 4 para una configuracion y una lista de casos
// cada caso trae un valor por variable de entrada, en el mismo orden del archivo de variables

static void probar(const std::string& titulo, const std::string& archVar, const std::string& archReglas, const std::vector<std::vector<double>>& casos){
    std::cout << titulo << std::endl;
    std::cout << "Fase 1: Variables linguisticas\n";
    std::vector<LinguisticVariable> variables = VariableCharger::cargar(archVar);
    for(const auto& v : variables) v.mostrar();

    std::cout << "Fase 2: Base de reglas\n";
    KnowledgeBase base(archReglas, variables); // no modificar variables despues de esto
    base.mostrarReglas();
    base.mostrarActivaciones();

    std::cout << "Fase 3: Motor de inferencia\n";
    // separa las entradas (en el orden del archivo) de la variable de salida
    std::vector<LinguisticVariable*> entradas;
    LinguisticVariable* salida = nullptr;
    for(auto& v : variables){
        if(v.getTipo() == VariableType::ENTRADA) entradas.push_back(&v);
        else salida = &v;
    }
    if(salida == nullptr) throw std::runtime_error("'" + archVar + "' no define una variable de salida");
    InferenceEngine motor(base, entradas, *salida);

    std::cout << "Fase 4: Validacion\n";
    for (const auto& c : casos){
        std::cout << "- Caso:";
        for(size_t i = 0; i < c.size() && i < entradas.size(); i++){
            std::cout << (i == 0 ? " " : ", ") << entradas[i]->getNombre() << " = " << c[i];
        }
        double r = motor.inferir(c);
        std::cout << "\nResultado final: " << r << " " << salida->getUnidad() << "\n";
    }
}

int main(){
    // casos del frenado: {Distancia en m, Velocidad en k/h, Via en %}, mismo orden del archivo de variables
    std::vector<std::vector<double>> casos = {{100, 5, 75}, {60, 30, 30}, {10, 90, 75}, {45, 20, 55}, {80, 85, 30}};
    probar("Frenado vehiculo autonomo 3x3x2 -> 5", "data-frenado/variables.txt", "data-frenado/rules.txt", casos);

    return 0;
}
