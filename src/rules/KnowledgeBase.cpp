#include "KnowledgeBase.h"
#include "../utils/text.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

// constructor vacio
KnowledgeBase::KnowledgeBase(): varEntrada1(nullptr), varEntrada2(nullptr), varEntrada3(nullptr), varSalida(nullptr) {}

// constructor que carga el archivo 
KnowledgeBase::KnowledgeBase(const std::string& rutaArchivo, const std::vector<LinguisticVariable>& variables): varEntrada1(nullptr), varEntrada2(nullptr), varEntrada3(nullptr), varSalida(nullptr) {
    cargarDesdeArchivo(rutaArchivo, variables);
}

// vacia el vector y reinicia los punteros
void KnowledgeBase::limpiar() {
    reglas.clear();
    varsEntrada.clear();
    varEntrada1 = nullptr;
    varEntrada2 = nullptr;
    varEntrada3 = nullptr;
    varSalida = nullptr;
}

// añade una regla sin validarla
void KnowledgeBase::agregarRegla(const Rule& regla) {
    reglas.push_back(regla);
}

// cortar los comentarios
std::string KnowledgeBase::quitarComentarios(const std::string& linea) {
    std::size_t pos = linea.find('#');
    return (pos != std::string::npos) ? linea.substr(0, pos) : linea;
}

// separa la linea en palabras usando los espacios (>> salta espacios, tabulaciones y '\r')
std::vector<std::string> KnowledgeBase::tokenizar(const std::string& linea) {
    std::istringstream flujo(linea);
    std::vector<std::string> tokens;
    std::string token;
    while (flujo >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// busca la variable por nombre sin importar mayusculas
const LinguisticVariable* KnowledgeBase::buscarVariable(
    const std::string& nombre,
    const std::vector<LinguisticVariable>& variables) const {
    for (const auto& var : variables) {
        if (iguales(var.getNombre(), nombre)) {
            return &var; // direccion de la variable dentro del vector que se recibio
        }
    }
    return nullptr;
}

// valida una parte "Variable ES Conjunto" de la regla y crea la proposicion
Proposition KnowledgeBase::validarYCrearProposicion( const std::string& nombreVar, const std::string& nombreConjunto, VariableType tipoEsperado, const std::vector<LinguisticVariable>& variables, int numLinea) {
    // la variable debe existir en el archivo de variables
    const LinguisticVariable* var = buscarVariable(nombreVar, variables);
    if (!var) {
        throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": La variable '" + nombreVar + "' no existe en el sistema.");
    }

    // los antecedentes deben ser de entrada y el consecuente de salida
    if (var->getTipo() != tipoEsperado) {
        std::string tipoEsperadoStr = (tipoEsperado == VariableType::ENTRADA) ? "ENTRADA" : "SALIDA";
        std::string tipoActualStr = tipoATexto(var->getTipo());
        throw std::invalid_argument("Línea " + std::to_string(numLinea) +": La variable '" + nombreVar + "' es de tipo " + tipoActualStr + ", pero se esperaba una variable de " + tipoEsperadoStr + ".");
    }

    // el conjunto debe pertenecer a esa variable
    const DiffuseSet* conjunto = var->getConjunto(nombreConjunto);
    if (!conjunto) {
        throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": El conjunto '" + nombreConjunto + "' no pertenece a la variable '" + nombreVar + "'.");
    }

    // se crea con los nombres oficiales y los punteros reales, aca la regla queda conectada a los conjuntos
    return Proposition(var->getNombre(), conjunto->getNombre(), conjunto, var);
}

// lee el archivo de reglas linea por linea, valida cada regla y la guarda
void KnowledgeBase::cargarDesdeArchivo(const std::string& rutaArchivo, const std::vector<LinguisticVariable>& variables) {
    limpiar(); // se borran las reglas anteriores

    //apertura del archivo
    std::ifstream archivo(rutaArchivo);
    if (!archivo) {
        throw std::runtime_error("No se pudo abrir el archivo de reglas '" + rutaArchivo + "'");
    }

    std::string linea;
    int numLinea = 0; // contador de lineas para los mensajes de error

    while (std::getline(archivo, linea)) {
        numLinea++;
        //quita el bom por si acaso (bytes invisibles que algunos editores ponen al inicio)
        if (numLinea == 1 && linea.rfind("\xEF\xBB\xBF", 0) == 0) {
            linea.erase(0, 3);
        }

        // quita el comentario y separa la linea en palabras
        std::vector<std::string> tokens = tokenizar(quitarComentarios(linea));
        if (tokens.empty()) {
            continue; // Línea vacía o solo comentario
        }

        // Formato esperado:
        // SI <Var1> ES <Conj1> Y <Var2> ES <Conj2> [Y <Var3> ES <Conj3>] ENTONCES <VarSalida> ES <ConjSalida>
        if (!iguales(tokens[0], "SI")) {
            throw std::runtime_error("Error en " + rutaArchivo + ", línea " + std::to_string(numLinea) + ": La regla debe comenzar con 'SI'.");
        }

        // Buscar la palabra clave ENTONCES
        int posEntonces = -1;
        for (std::size_t i = 1; i < tokens.size(); ++i) {
            if (iguales(tokens[i], "ENTONCES")) {
                posEntonces = static_cast<int>(i);
                break;
            }
        }

        if (posEntonces == -1) {
            throw std::runtime_error("Error en " + rutaArchivo + ", línea " + std::to_string(numLinea) + ": Formato de regla inválido. No se encontró la palabra clave 'ENTONCES'.");
        }

        // Después de ENTONCES debe haber exactamente 3 tokens: <VarSalida> ES <ConjuntoSalida>
        if (static_cast<std::size_t>(posEntonces + 4) != tokens.size() || !iguales(tokens[posEntonces + 2], "ES")) {
            throw std::runtime_error("Error en " + rutaArchivo + ", línea " + std::to_string(numLinea) + ": Formato de consecuente inválido. Se esperaba: ENTONCES <VarSalida> ES <ConjuntoSalida>.");
        }

        try {
            // Parsear los antecedentes entre el token 1 y posEntonces
            std::vector<Proposition> antecedentes;
            std::size_t idx = 1;
            while (idx < static_cast<std::size_t>(posEntonces)) {
                if (idx + 2 >= static_cast<std::size_t>(posEntonces)) {
                    throw std::runtime_error("Línea " + std::to_string(numLinea) + ": Formato de antecedente incompleto antes de 'ENTONCES'.");
                }
                std::string varNombre = tokens[idx];
                std::string esPalabra = tokens[idx + 1];
                std::string conjNombre = tokens[idx + 2];

                if (!iguales(esPalabra, "ES")) {
                    throw std::runtime_error("Línea " + std::to_string(numLinea) + ": Se esperaba 'ES' después de '" + varNombre + "', pero se encontró '" + esPalabra + "'.");
                }

                antecedentes.push_back(validarYCrearProposicion(varNombre, conjNombre, VariableType::ENTRADA, variables, numLinea));

                idx += 3;
                if (idx < static_cast<std::size_t>(posEntonces)) {
                    if (!iguales(tokens[idx], "Y")) {
                        throw std::runtime_error("Línea " + std::to_string(numLinea) + ": Se esperaba 'Y' entre antecedentes, pero se encontró '" + tokens[idx] + "'.");
                    }
                    idx += 1;
                }
            }

            if (antecedentes.empty()) {
                throw std::runtime_error("Línea " + std::to_string(numLinea) + ": La regla no contiene antecedentes.");
            }

            // No pueden repetirse variables de entrada dentro de los antecedentes de la misma regla
            for (std::size_t i = 0; i < antecedentes.size(); ++i) {
                for (std::size_t j = i + 1; j < antecedentes.size(); ++j) {
                    if (iguales(antecedentes[i].getVariableNombre(), antecedentes[j].getVariableNombre())) {
                        throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": Los antecedentes no pueden repetir la misma variable de entrada ('" + antecedentes[i].getVariableNombre() + "').");
                    }
                }
            }

            // Validar consecuente (Salida)
            Proposition consec = validarYCrearProposicion(tokens[posEntonces + 1], tokens[posEntonces + 3], VariableType::SALIDA, variables, numLinea);

            // Asegurar consistencia entre las reglas cargadas
            if (varsEntrada.empty() && !varSalida) {
                // primera regla: se guardan las variables que usa el sistema
                for (const auto& ant : antecedentes) {
                    varsEntrada.push_back(ant.getVariable());
                }
                if (varsEntrada.size() >= 1) varEntrada1 = varsEntrada[0];
                if (varsEntrada.size() >= 2) varEntrada2 = varsEntrada[1];
                if (varsEntrada.size() >= 3) varEntrada3 = varsEntrada[2];
                varSalida = consec.getVariable();
            } else {
                // las demas reglas deben tener la misma cantidad de antecedentes y usar las mismas variables de entrada
                if (antecedentes.size() != varsEntrada.size()) {
                    throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": Regla inconsistente. Se esperaban " + std::to_string(varsEntrada.size()) + " antecedentes, pero tiene " + std::to_string(antecedentes.size()) + ".");
                }

                for (const auto& ant : antecedentes) {
                    bool encontrada = false;
                    for (const auto* vEnt : varsEntrada) {
                        if (iguales(ant.getVariableNombre(), vEnt->getNombre())) {
                            encontrada = true;
                            break;
                        }
                    }
                    if (!encontrada) {
                        throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": Regla inconsistente con las variables de entrada definidas anteriormente.");
                    }
                }

                if (!iguales(consec.getVariableNombre(), varSalida->getNombre())) {
                    throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": Regla inconsistente con la variable de salida definida anteriormente (" + varSalida->getNombre() + ").");
                }
            }

            // el id es la cantidad de reglas + 1 (R1, R2...)
            int id = static_cast<int>(reglas.size()) + 1;
            reglas.emplace_back(id, antecedentes, consec);

        } catch (const std::invalid_argument& e) {
            // se agrega el nombre del archivo para que el error sea facil de ubicar
            throw std::runtime_error("Error en " + rutaArchivo + ": " + e.what());
        }
    }

    // el archivo debe tener al menos una regla
    if (reglas.empty()) {
        throw std::runtime_error("Error en " + rutaArchivo + ": No se encontraron reglas válidas en el archivo.");
    }
}

// muestra la tabla de reglas cargadas con las variables del sistema
void KnowledgeBase::mostrarReglas() const {

    std::cout << "\n" << " Base de conocimiento difusa (" << reglas.size() << " reglas cargadas)\n";

    // variables del sistema si ya se cargaron reglas
    for (std::size_t i = 0; i < varsEntrada.size(); ++i) {
        std::cout << " Entrada " << (i + 1) << " : " << varsEntrada[i]->getNombre() << " (" << varsEntrada[i]->getUnidad() << ")\n";
    }
    if (varSalida) {
        std::cout << " Salida    : " << varSalida->getNombre() << " (" << varSalida->getUnidad() << ")\n";
    }

    // encabezado de la tabla
    std::cout << "\n   " << std::left << std::setw(6) << "ID";
    for (std::size_t i = 0; i < varsEntrada.size(); ++i) {
        std::string colNombre = "Antecedente " + std::to_string(i + 1);
        std::cout << std::setw(26) << colNombre;
    }
    std::cout << std::setw(26) << "Consecuente" << "\n";

    // una fila por regla
    for (const auto& r : reglas) {
        std::string idStr = "R" + std::to_string(r.getId());
        std::cout << "   " << std::left << std::setw(6) << idStr;
        for (const auto& ant : r.getAntecedentes()) {
            std::cout << std::setw(26) << ant.toString();
        }
        std::cout << std::setw(26) << r.getConsecuente().toString() << "\n";
    }
   
}

// muestra el alpha de cada regla usando los grados de la ultima fuzzificacion
void KnowledgeBase::mostrarActivaciones() const {
    std::size_t anchoRegla = (varsEntrada.size() >= 3) ? 110 : 80;

    std::cout << "\n Activacion de reglas\n\n";
    std::cout << "   " << std::left << std::setw(6) << "ID" << std::setw(anchoRegla) << "Regla" << std::setw(10) << "alpha   (Activacion)" << "\n";

    for (const auto& r : reglas) {
        std::string idStr = "R" + std::to_string(r.getId());
        double alpha = r.calcularActivacion(); // min de todos los antecedentes
        std::cout << "   " << std::left << std::setw(6) << idStr << std::setw(anchoRegla) << r.toString() << std::setw(10) << formatear(alpha, 4) << (alpha > 0.0 ? "   *" : "") << "\n"; // * si la regla se activo
    }
}
