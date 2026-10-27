#include "KnowledgeBase.h"
#include "../utils/text.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

// constructor vacio
KnowledgeBase::KnowledgeBase(): varEntrada1(nullptr), varEntrada2(nullptr), varSalida(nullptr) {}

// constructor que carga el archivo 
KnowledgeBase::KnowledgeBase(const std::string& rutaArchivo, const std::vector<LinguisticVariable>& variables): varEntrada1(nullptr), varEntrada2(nullptr), varSalida(nullptr) {
    cargarDesdeArchivo(rutaArchivo, variables);
}

// vacia el vector y reinicia los punteros
void KnowledgeBase::limpiar() {
    reglas.clear();
    varEntrada1 = nullptr;
    varEntrada2 = nullptr;
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
        // SI <Var1> ES <Conjunto1> Y <Var2> ES <Conjunto2> ENTONCES <VarSalida> ES <ConjuntoSalida>
        if (tokens.size() != 12) {
            throw std::runtime_error("Error en " + rutaArchivo + ", línea " + std::to_string(numLinea) + ": Formato de regla inválido. Se esperaba: " "SI <Var1> ES <Conjunto1> Y <Var2> ES <Conjunto2> ENTONCES <VarSalida> ES <ConjuntoSalida>");
        }

        // palabras clave en las posiciones 0, 2, 4, 6, 8 y 10 sin importar mayusculas
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
                // primera regla: se guardan las variables que usa el sistema
                varEntrada1 = ant1.getVariable();
                varEntrada2 = ant2.getVariable();
                varSalida = consec.getVariable();
            } else {
                // las demas reglas deben usar las mismas entradas (en cualquier orden) y la misma salida
                bool ordenDirecto = (iguales(ant1.getVariableNombre(), varEntrada1->getNombre()) &&  iguales(ant2.getVariableNombre(), varEntrada2->getNombre()));
                bool ordenInverso = (iguales(ant1.getVariableNombre(), varEntrada2->getNombre()) && iguales(ant2.getVariableNombre(), varEntrada1->getNombre()));

                if (not ordenDirecto && not ordenInverso) {
                    throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": Regla inconsistente con las variables de entrada definidas anteriormente (" + varEntrada1->getNombre() + ", " + varEntrada2->getNombre() + ").");
                }

                if (!iguales(consec.getVariableNombre(), varSalida->getNombre())) {
                    throw std::invalid_argument("Línea " + std::to_string(numLinea) + ": Regla inconsistente con la variable de salida definida anteriormente (" + varSalida->getNombre() + ").");
                }
            }

            // el id es la cantidad de reglas + 1 (R1, R2...)
            int id = static_cast<int>(reglas.size()) + 1;
            reglas.emplace_back(id, ant1, ant2, consec); // construye la regla directamente dentro del vector

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
    if (varEntrada1 && varEntrada2 && varSalida) {
        std::cout << " Entrada 1 : " << varEntrada1->getNombre() << " (" << varEntrada1->getUnidad() << ")\n" << " Entrada 2 : " << varEntrada2->getNombre() << " (" << varEntrada2->getUnidad() << ")\n" << " Salida    : " << varSalida->getNombre() << " (" << varSalida->getUnidad() << ")\n";
    }

    // encabezado de la tabla
    std::cout << "\n" << "   " << std::left << std::setw(6)  << "ID" << std::setw(26) << "Antecedente 1" << std::setw(26) << "Antecedente 2" << std::setw(26) << "Consecuente"  << "\n";

    // una fila por regla
    for (const auto& r : reglas) {
        std::string idStr = "R" + std::to_string(r.getId());
        std::cout << "   " << std::left << std::setw(6)  << idStr << std::setw(26) << r.getAntecedente1().toString() << std::setw(26) << r.getAntecedente2().toString() << std::setw(26) << r.getConsecuente().toString() << "\n";
    }
   
}

// muestra el alpha de cada regla usando los grados de la ultima fuzzificacion
void KnowledgeBase::mostrarActivaciones() const {

    std::cout << "\n" << " Activacion de reglas\n" << "\n" << "   " << std::left << std::setw(6)  << "ID" << std::setw(74) << "Regla" << std::setw(25) << "alpha (Activacion)" << "\n";

    for (const auto& r : reglas) {
        std::string idStr = "R" + std::to_string(r.getId());
        double alpha = r.calcularActivacion(); // min de los dos antecedentes
        std::cout << "   " << std::left  << std::setw(6)  << idStr << std::setw(74) << r.toString() << std::setw(25) << formatear(alpha, 4) << (alpha > 0.0 ? " *" : "") << "\n"; // * si la regla se activo
    }
}
