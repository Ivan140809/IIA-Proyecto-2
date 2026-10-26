#include "KnowledgeBase.h"
#include "../utils/text.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

KnowledgeBase::KnowledgeBase()
    : varEntrada1(nullptr), varEntrada2(nullptr), varSalida(nullptr) {}

KnowledgeBase::KnowledgeBase(const std::string& rutaArchivo, const std::vector<LinguisticVariable>& variables): varEntrada1(nullptr), varEntrada2(nullptr), varSalida(nullptr) {
    cargarDesdeArchivo(rutaArchivo, variables);
}

void KnowledgeBase::limpiar() {
    reglas.clear();
    varEntrada1 = nullptr;
    varEntrada2 = nullptr;
    varSalida = nullptr;
}

void KnowledgeBase::agregarRegla(const Rule& regla) {
    reglas.push_back(regla);
}

std::string KnowledgeBase::quitarComentarios(const std::string& linea) {
    std::size_t pos = linea.find('#');
    return (pos != std::string::npos) ? linea.substr(0, pos) : linea;
}

std::vector<std::string> KnowledgeBase::tokenizar(const std::string& linea) {
    std::istringstream flujo(linea);
    std::vector<std::string> tokens;
    std::string token;
    while (flujo >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

const LinguisticVariable* KnowledgeBase::buscarVariable(
    const std::string& nombre,
    const std::vector<LinguisticVariable>& variables) const {
    for (const auto& var : variables) {
        if (iguales(var.getNombre(), nombre)) {
            return &var;
        }
    }
    return nullptr;
}

Proposition KnowledgeBase::validarYCrearProposicion( const std::string& nombreVar, const std::string& nombreConjunto, VariableType tipoEsperado, const std::vector<LinguisticVariable>& variables, int numLinea) {

    const LinguisticVariable* var = buscarVariable(nombreVar, variables);
    if (!var) {
        throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": La variable '" + nombreVar + "' no existe en el sistema.");
    }

    if (var->getTipo() != tipoEsperado) {
        std::string tipoEsperadoStr = (tipoEsperado == VariableType::ENTRADA) ? "ENTRADA" : "SALIDA";
        std::string tipoActualStr = tipoATexto(var->getTipo());
        throw std::invalid_argument("Línea " + std::to_string(numLinea) +": La variable '" + nombreVar + "' es de tipo " + tipoActualStr + ", pero se esperaba una variable de " + tipoEsperadoStr + ".");
    }

    const DiffuseSet* conjunto = var->getConjunto(nombreConjunto);
    if (!conjunto) {
        throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": El conjunto '" + nombreConjunto + "' no pertenece a la variable '" + nombreVar + "'.");
    }
    return Proposition(var->getNombre(), conjunto->getNombre(), conjunto, var);
}

void KnowledgeBase::cargarDesdeArchivo(const std::string& rutaArchivo, const std::vector<LinguisticVariable>& variables) {
    limpiar();
    std::ifstream archivo(rutaArchivo);
    if (!archivo) {
        throw std::runtime_error("No se pudo abrir el archivo de reglas '" + rutaArchivo + "'");
    }

    std::string linea;
    int numLinea = 0;

    while (std::getline(archivo, linea)) {
        numLinea++;
        if (numLinea == 1 && linea.rfind("\xEF\xBB\xBF", 0) == 0) {
            linea.erase(0, 3);
        }

        std::vector<std::string> tokens = tokenizar(quitarComentarios(linea));
        if (tokens.empty()) {
            continue; // Línea vacía o solo comentario
        }

        // Formato esperado:
        // SI <Var1> ES <Conjunto1> Y <Var2> ES <Conjunto2> ENTONCES <VarSalida> ES <ConjuntoSalida>
        if (tokens.size() != 12) {
            throw std::runtime_error("Error en " + rutaArchivo + ", línea " + std::to_string(numLinea) + ": Formato de regla inválido. Se esperaba: " "SI <Var1> ES <Conjunto1> Y <Var2> ES <Conjunto2> ENTONCES <VarSalida> ES <ConjuntoSalida>");
        }

        if (!iguales(tokens[0], "SI") || !iguales(tokens[2], "ES") || !iguales(tokens[4], "Y") || !iguales(tokens[6], "ES") ||!iguales(tokens[8], "ENTONCES") || !iguales(tokens[10], "ES")) {
            throw std::runtime_error("Error en " + rutaArchivo + ", línea " + std::to_string(numLinea) + ": Palabras clave incorrectas. Debe seguir la estructura: " "SI ... ES ... Y ... ES ... ENTONCES ... ES ...");
        }

        try {
            // Validar antecedente 1 (Entrada)
            Proposition ant1 = validarYCrearProposicion(tokens[1], tokens[3], VariableType::ENTRADA, variables, numLinea);

            // Validar antecedente 2 (Entrada)
            Proposition ant2 = validarYCrearProposicion(tokens[5], tokens[7], VariableType::ENTRADA, variables, numLinea);

            // No pueden ser la misma variable de entrada
            if (iguales(ant1.getVariableNombre(), ant2.getVariableNombre())) {
                throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": Los antecedentes deben corresponder a dos variables de entrada distintas.");
            }

            // Validar consecuente (Salida)
            Proposition consec = validarYCrearProposicion(tokens[9], tokens[11], VariableType::SALIDA, variables, numLinea);

            // Asegurar consistencia entre las reglas (el sistema modela 2 entradas y 1 salida específicas)
            if (!varEntrada1 && !varEntrada2 && !varSalida) {
                varEntrada1 = ant1.getVariable();
                varEntrada2 = ant2.getVariable();
                varSalida = consec.getVariable();
            } else {
                bool ordenDirecto = (iguales(ant1.getVariableNombre(), varEntrada1->getNombre()) &&  iguales(ant2.getVariableNombre(), varEntrada2->getNombre()));
                bool ordenInverso = (iguales(ant1.getVariableNombre(), varEntrada2->getNombre()) && iguales(ant2.getVariableNombre(), varEntrada1->getNombre()));

                if (not ordenDirecto && not ordenInverso) {
                    throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": Regla inconsistente con las variables de entrada definidas anteriormente (" + varEntrada1->getNombre() + ", " + varEntrada2->getNombre() + ").");
                }

                if (!iguales(consec.getVariableNombre(), varSalida->getNombre())) {
                    throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": Regla inconsistente con la variable de salida definida anteriormente (" + varSalida->getNombre() + ").");
                }
            }

            int id = static_cast<int>(reglas.size()) + 1;
            reglas.emplace_back(id, ant1, ant2, consec);

        } catch (const std::invalid_argument& e) {
            throw std::runtime_error("Error en " + rutaArchivo + ": " + e.what());
        }
    }

    if (reglas.empty()) {
        throw std::runtime_error("Error en " + rutaArchivo + ": No se encontraron reglas válidas en el archivo.");
    }
}

void KnowledgeBase::mostrarReglas() const {

    std::cout << "\n" << " Base de conocimiento difusa (" << reglas.size() << " reglas cargadas)\n";

    if (varEntrada1 && varEntrada2 && varSalida) {
        std::cout << " Entrada 1 : " << varEntrada1->getNombre() << " (" << varEntrada1->getUnidad() << ")\n"
                  << " Entrada 2 : " << varEntrada2->getNombre() << " (" << varEntrada2->getUnidad() << ")\n"
                  << " Salida    : " << varSalida->getNombre() << " (" << varSalida->getUnidad() << ")\n";
    }

    std::cout << "\n" << "   " << std::left << std::setw(6)  << "ID" << std::setw(26) << "Antecedente 1" << std::setw(26) << "Antecedente 2" << std::setw(26) << "Consecuente"  << "\n";

    for (const auto& r : reglas) {
        std::string idStr = "R" + std::to_string(r.getId());
        std::cout << "   " << std::left << std::setw(6)  << idStr << std::setw(26) << r.getAntecedente1().toString() << std::setw(26) << r.getAntecedente2().toString() << std::setw(26) << r.getConsecuente().toString() << "\n";
    }
   
}

void KnowledgeBase::mostrarActivaciones() const {

    std::cout << "\n" << " ACTIVACIÓN DE REGLAS (Fuerza de Activacion alpha = min(mu1, mu2))\n" << "\n"
              << "   " << std::left
              << std::setw(6)  << "ID"
              << std::setw(74) << "Regla"
              << std::setw(14) << "alpha (Activacion)"
              << "\n";

    for (const auto& r : reglas) {
        std::string idStr = "R" + std::to_string(r.getId());
        double alpha = r.calcularActivacion();
        std::cout << "   " << std::left  << std::setw(6)  << idStr << std::setw(74) << r.toString() << std::setw(14) << formatear(alpha, 4) << (alpha > 0.0 ? " *" : "") << "\n";
    }
}
