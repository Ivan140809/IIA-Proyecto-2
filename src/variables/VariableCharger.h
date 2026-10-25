#ifndef VARIABLE_CHARGER_H
#define VARIABLE_CHARGER_H

#include "LinguisticVariable.h"

#include <memory>
#include <string>
#include <vector>


/**
 * Lee el archivo de texto que define las variables lingüísticas del
 * sistema y crea las instancias de LinguisticVariable y DiffuseSet.
 * 
 * cada linea conjunto pertenece a la ultima variable declarada
 * en un mismo archivo van en una variable del sistema
 */

 class VariableCharger {
    public:
    /**
     * Carga todas las variables definidas en el archivo.
     * retorna las variables en el orden que aparecen en el archivo
     */
    static std::vector<LinguisticVariable> cargar(const std::string& ruta);

    private:
    static LinguisticVariable crearVariable(const std::vector<std::string>& campos);
    static std::unique_ptr<DiffuseSet> crearConjunto(const std::vector<std::string>& campos);

    /** Verifica que la línea tenga exactamente la cantidad de campos esperada */
    static void exigirCampos(const std::vector<std::string>& campos, std::size_t esperados, const std::string& formato);

    /** Separa una línea en palabras usando los espacios */
    static std::vector<std::string> separarCampos(const std::string& linea);

    static std::string quitarComentario(const std::string& linea);
};

#endif
