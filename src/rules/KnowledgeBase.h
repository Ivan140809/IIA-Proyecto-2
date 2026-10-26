#ifndef KNOWLEDGE_BASE_H
#define KNOWLEDGE_BASE_H

#include "Rule.h"
#include "../variables/LinguisticVariable.h"
#include <string>
#include <vector>

/**
 * Representa la Base de Conocimiento de un sistema difuso para gestionar
 * inferencias con dos variables de entrada y una de salida.
 *
 * Se encarga de:
 * - Cargar reglas lingüísticas desde un archivo de texto.
 * - Validar sintáctica y semánticamente las reglas contra las variables de Fase 1.
 * - Almacenar y visualizar las reglas difusas en formato texto.
 */
class KnowledgeBase {

    std::vector<Rule> reglas;
    const LinguisticVariable* varEntrada1;
    const LinguisticVariable* varEntrada2;
    const LinguisticVariable* varSalida;

    /** Métodos auxiliares de parseo y validación */
    static std::string quitarComentarios(const std::string& linea);
    static std::vector<std::string> tokenizar(const std::string& linea);

    const LinguisticVariable* buscarVariable(const std::string& nombre, const std::vector<LinguisticVariable>& variables) const;
    Proposition validarYCrearProposicion(const std::string& nombreVar, const std::string& nombreConjunto, VariableType tipoEsperado, const std::vector<LinguisticVariable>& variables, int numLinea);
public:
    KnowledgeBase();
    KnowledgeBase(const std::string& rutaArchivo, const std::vector<LinguisticVariable>& variables);

    /**
     * Carga y valida las reglas difusas desde el archivo indicado,
     * conectándolas con las variables lingüísticas.
     */
    void cargarDesdeArchivo(const std::string& rutaArchivo, const std::vector<LinguisticVariable>& variables);

    /** Agrega manualmente una regla validada */
    void agregarRegla(const Rule& regla);

    /** Limpia las reglas almacenadas */
    void limpiar();

    /** Muestra en consola la lista formateada de reglas cargadas */
    void mostrarReglas() const;

    /**
     * Muestra la fuerza de disparo (activación alpha) de cada regla
     * en función de las variables actualmente fuzzificadas.
     */
    void mostrarActivaciones() const;

    const std::vector<Rule>& getReglas() const { return reglas; }
    std::size_t getCantidadReglas() const { return reglas.size(); }
    bool estaVacia() const { return reglas.empty(); }

    const LinguisticVariable* getVariableEntrada1() const { return varEntrada1; }
    const LinguisticVariable* getVariableEntrada2() const { return varEntrada2; }
    const LinguisticVariable* getVariableSalida() const { return varSalida; }

};

#endif
