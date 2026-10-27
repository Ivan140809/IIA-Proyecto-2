#include "VariableCharger.h"
#include "../sets/TrapezoidalSet.h"
#include "../sets/TriangularSet.h"
#include "../utils/text.h"
#include <fstream>
#include <sstream>
#include <stdexcept>


std::vector<LinguisticVariable> VariableCharger::cargar(const std::string& ruta) {
    //apertura del archivo
    std::ifstream archivo(ruta);
    if (!archivo) {
        throw std::runtime_error("no se pudo abrir el archivo '" + ruta + "'");
    }
    std::vector<LinguisticVariable> variables;
    std::string linea;
    int numLinea = 0;
    while (std::getline(archivo, linea)) {
        //contador de lineas
        numLinea++;
        //quita el bom por si acaso
        if (numLinea == 1 && linea.rfind("\xEF\xBB\xBF", 0) == 0) {
            linea.erase(0, 3);
        }
        std::vector<std::string> campos = separarCampos(quitarComentario(linea));
        if (campos.empty()) {
            continue; // linea vacia o solo comentario
        }
        try {
            std::string palabraClave = aMayus(campos[0]);
            if (palabraClave == "VARIABLE") {
                variables.push_back(crearVariable(campos)); //si es variable la crea y mete al final del vector
            } else if (palabraClave == "CONJUNTO") { // si es conjunto busca una variable asociada si no da error 
                if (variables.empty()) {
                    throw std::invalid_argument("Conjunto declarado antes de cualquier Variable");
                }
                // el conjunto se agrega a la ultima variable declarada
                variables.back().agregarConjunto(crearConjunto(campos));
            } else {
                throw std::invalid_argument("palabra clave '" + campos[0] + "' desconocida (use VARIABLE o CONJUNTO)");
            }
        } catch (const std::invalid_argument& e) {
            // se agrega el numero de linea para que el error sea facil de ubicar en el archivo
            throw std::runtime_error("Error en " + ruta + ", línea " + std::to_string(numLinea) + ": " + e.what());
        }
    }

    // Toda variable debe tener al menos un conjunto difuso para ser valido
    for (const auto& v : variables) {
        if (v.getConjuntos().empty()) {
            throw std::runtime_error("Error en " + ruta + ": la variable '" + v.getNombre() + "' no tiene conjuntos");
        }
    }
    return variables; // devuelve el vector al comprobar que haya un conjunto difuso
}

// VARIABLE <nombre> <ENTRADA|SALIDA> <min> <max> <unidad>
//exige estos 6 campos para construir la variable dentro del archivo de variables
// "VARIABLE","Descuento","ENTRADA","0","50","%"
LinguisticVariable VariableCharger::crearVariable(const std::vector<std::string>& campos) {
    exigirCampos(campos, 6, "Variable<nombre> <ENTRADA|SALIDA> <min> <max> <unidad>");
    return LinguisticVariable(campos[1], tipoDesdeTexto(campos[2]), aNumero(campos[3]), aNumero(campos[4]), campos[5]);
}

// exige 3 campos antes de leer los puntos
std::unique_ptr<DiffuseSet> VariableCharger::crearConjunto(const std::vector<std::string>& campos) {
    if (campos.size() < 3) {
        throw std::invalid_argument("formato esperado: Conjunto <nombre> <TRIANGULAR|TRAPEZOIDAL> <puntos...>");
    }
    const std::string& nombre = campos[1];
    std::string forma = aMayus(campos[2]);
    // 6 campos exactos para ser triangular y poder crear el conjunto triangular
    if (forma == "TRIANGULAR") {
        exigirCampos(campos, 6, "CONJUNTO <nombre> TRIANGULAR <a> <b> <c>");
        return std::make_unique<TriangularSet>(nombre, aNumero(campos[3]), aNumero(campos[4]), aNumero(campos[5]));
    }
    // 7 campos exactamente
    if (forma == "TRAPEZOIDAL") {
        exigirCampos(campos, 7, "CONJUNTO <nombre> TRAPEZOIDAL <a> <b> <c> <d>");
        return std::make_unique<TrapezoidalSet>(nombre, aNumero(campos[3]), aNumero(campos[4]), aNumero(campos[5]), aNumero(campos[6]));
    }
    //cualquier otra forma da error
    throw std::invalid_argument("forma '" + campos[2] + "' desconocida (use TRIANGULAR o TRAPEZOIDAL)");
}

// lanza un error con el formato si los campos no coinciden
void VariableCharger::exigirCampos(const std::vector<std::string>& campos, std::size_t esperados, const std::string& formato) {
    if (campos.size() != esperados) {
        throw std::invalid_argument("formato esperado: " + formato);
    }
}

// lee una palabra y salta espacios del archivo y tabulaciones
std::vector<std::string> VariableCharger::separarCampos(const std::string& linea) {
    std::istringstream flujo(linea);
    std::vector<std::string> campos;
    std::string campo;
    while (flujo >> campo) {   // >> salta espacios, tabulaciones y '\r'
        campos.push_back(campo);
    }
    return campos;
}

// elimina todo lo que sigue a '#' en la linea
std::string VariableCharger::quitarComentario(const std::string& linea) {
    std::size_t pos = linea.find('#');
    return (pos != std::string::npos) ? linea.substr(0, pos) : linea;
}