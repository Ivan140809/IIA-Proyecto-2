#ifndef DIFFUSE_SET_H
#define DIFFUSE_SET_H

#include <initializer_list> 
#include <ostream>
#include <string>

/**
 * Este archivo representa un conjunto difuso de una variable linguistica de entrada. se basa en una funcion 
 * de pertenencia indicando que grado un valor pertenece al conjunto de 0 a 1.
 * 
 * El conjunto se define como una clase abstracta cada forma concreta implementa su propia
 * funcion de pertenencia con el metodo sobreescribiendolo sin importar la entrada
 * 
 * EL conjunto registra el ultimo grado de pertenencia calculado en la fuzzificacion
 */

 class DiffuseSet {

public:
/** etiqueta linguistica del conjunto, explicit impide que se convierta un string en un conjunto difuso*/
    explicit DiffuseSet(const std::string& nombre);

/** Destructor para los conjuntos con un puntero dinamico,virtual se usa para el polimorfismo en la herencia */
    virtual ~DiffuseSet() = default;


/** Funcion de pertenencia calculando el valor sin modificar el estado del conjunto, agarra el valor del mundo real
 * y lo transforma en el grado de pertenencia del conjunto
 */

 /** virtual significa el polimorfismo en ejecucion, cada clase hija debe implementarla por eso DiffuseSet es una clase abstracta */
virtual double calcularPertenencia(double x) const = 0;

//
/** nombre de la forma triangular o trapezoidal*/
virtual std::string getTipo() const =0;

/** los puntos que definen la forma como texto (es decir los que llegan de entrada y definen la forma del conjunto) */
virtual std::string getPuntosTexto() const = 0;

/** Limite inferior izquierdo, indica donde empieza la figura */
virtual double getLimiteInferior() const =0;

/** Limite superior derecho, indica donde termina la figura */
virtual double getLimiteSuperior() const =0;


/*** Fuzzificacion del valor real, calcula el valor del conjunto y lo guarda como grado de pertenencia actual del conjunto
 * le entra el valor real de variable y le sale el grado de pertenencia calculado
 */
double fuzzificar(double x);
// restablecer grado de pertenencia a 0
void reiniciarGrado();

/** getters */
const std::string& getNombre() const;
double getGradoPertenencia() const;

protected:
    /** Convierte una lista de puntos en el texto, funcion estatica que no necesariamente acepta un objeto */
    static std::string formatearPuntos(std::initializer_list<double> puntos);


private:
    /** Etiqueta linguistica del conjunto ya sea bajo, medio o alto */
    std::string nombre;
    /** Ultimo grado de pertenencia calculado */
    double gradoPertenencia;

};

/** Escribe una línea con nombre, forma y puntos se usa al visualizar la variable. */
std::ostream& operator<<(std::ostream& os, const DiffuseSet& conjunto);
#endif