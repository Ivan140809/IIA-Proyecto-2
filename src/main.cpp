#include <iostream>
#include <string>
#include <vector>
#include "variables/VariableCharger.h"
#include "rules/KnowledgeBase.h"
#include "engine/InferenceEngine.h"

// corre las fases 1 a 4 para una configuracion y una lista de casos {entrada1, entrada2}

static void probar(const std::string& titulo, const std::string& archVar, const std::string& archReglas, const std::vector<std::pair<double, double>>& casos){
    std::cout << titulo << std::endl;
    std::cout << "Fase 1: Variables linguisticas\n";
    std::vector<LinguisticVariable> variables = VariableCharger::cargar(archVar);
    for(const auto& v : variables) v.mostrar();

    std::cout << "Fase 2: Base de reglas\n";
    KnowledgeBase base(archReglas, variables); // no modificar variables despues de esto
    base.mostrarReglas();
    base.mostrarActivaciones();

    std::cout << "Fase 3: Motor de inferencia\n";
    InferenceEngine motor(base, variables[0], variables[1], variables[2]);

    std::cout << "Fase 4: Validacion\n";
    for (const auto& c : casos){
        std::cout << "- Caso: " << variables[0].getNombre() << " = " << c.first<< ", " << variables[1].getNombre() << " = " << c.second;
        double r = motor.inferir(c.first, c.second);
        std::cout << "\nResultado final: " << r << " " << variables[2].getUnidad() << "\n";
    }
}

int main(){
    // casos del frenado: {Distancia en m, Velocidad en k/h}, mismo orden del archivo de variables
    std::vector<std::pair<double, double>> casos = {{100, 5}, {60, 30}, {10, 90}, {45, 20}, {80, 85}};
    probar("Frenado vehiculo autonomo 3x3 -> 5", "data-frenado/variables.txt", "data-frenado/rules.txt", casos);

    return 0;
}
