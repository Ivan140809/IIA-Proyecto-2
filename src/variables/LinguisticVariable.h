#ifndef LINGUISTIC_VARIABLE_H
#define LINGUISTIC_VARIABLE_H

#include "../sets/DiffuseSet.h"
#include "VariableType.h"
#include <memory>
#include <string>
#include <vector>

/**
 * Representa una variable linguistica la cual sus valores se expresan con etiquetas como en el caso implementado
 * donde los valores se expresan con palabras al establecer un descuento.
 * 
 * esto queda definido por un nombre un rango de porcentaje que representa el descuento y un conjunto de terminos que se modelan 
 * cada uno con una funcion de pertenencia
 * 
 * Se encarga de fizzificar, agrupar los valores y validarlos
 */

 class LinguisticVariable {
    public:
    /** lanza una excepcion si el universo es invalido (min>= max) */
    LinguisticVariable(const std::string &nombre, VariableType tipo, double minimo, double maximo, const std::string& unidad);

    /** Agrega un conjunto difuso a la variable y con ello la variable pasa a ser "dueña" del coniunto y cuando se destruye los conjuntos se liberan automaticamente
     * lanza una excepcion si ya existe un conjunto con ese nombre o si el conjunto se sale del universo
    */
    void agregarConjunto(std::unique_ptr<DiffuseSet> conjunto);

    /** Busca un conjunto por si etiqueta y retorna un puntero al conjunto
    */
    DiffuseSet* getConjunto(const std::string& nombreConjunto) const;

    /** Fuzzificacion, convierte el valor del mundo real en grado de pertenencia calculando su valor para cada uno
     * de los conjuntos y cada conjunto guarda internamente su grado
     * si el valor esta fuera de los valores posibles se une al limite mas cercano para que siempre pertenezca en algun grado
     */

    void fuzzificar(double valor);

    /** reinicia el valor y grados de todos los conjuntos */
    void reiniciar();

    /** Muestra la definición de la variable y de todos sus conjuntos. */
    void mostrar() const;

    /** Muestra el grado de pertenencia registrado en cada conjunto
     *  tras la ultima fuzzificación.
     * */
    void mostrarFuzzificacion() const;
    
    const std::string& getNombre() const {return nombre;}
    VariableType getTipo() const {return tipo;}
    double getMinimo() const {return minimo;}
    double getMaximo() const {return maximo;}
    const std::string& getUnidad() const {return unidad;}
    double getValorNitido() const {return valorNitido;}

    /** Acceso de lectura para los conjuntos */
    const std::vector <std::unique_ptr<DiffuseSet>>& getConjuntos() const {
        return conjuntos;
    }

private:
    std::string nombre;
    VariableType tipo;
    double minimo; // limite inferior del universo
    double maximo; // limite superior del universo 
    std::string unidad; // Unidad de medida

    /** Terminos linguisticos de la variable, se guardan como punteros a la clase base */
    std::vector<std::unique_ptr<DiffuseSet>> conjuntos;
    /** Ultimo valor fuzzificado */
    double valorNitido;
};
 #endif