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
    // mismos casos de entrada sobre configuraciones con distinta cantidad de conjuntos
    std::vector<std::pair<double, double>> casos = {{5, 1}, {25, 5}, {45, 9}, {15, 3}, {45, 1}};
    probar("3 conjuntos por variable", "data/variables.txt", "data/rules.txt", casos);
    probar("5 conjuntos por variable", "data/variables_5conjuntos.txt", "data/rules_5conjuntos.txt", casos);
    probar("Mixto 3x5 -> 4", "data/variables_mixto.txt", "data/rules_mixto.txt", casos);

    return 0;
}
