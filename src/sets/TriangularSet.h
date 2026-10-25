#ifndef TRIANGULAR_SET_H
#define TRIANGULAR_SET_H

#include "DiffuseSet.h"

/** conjunto difuso con funcion de pertenencia triangular definida por los tres puntos
 * y que tiene su definicion su traducion directa con el pico en la traduccion del conjunto.
 */

 class TriangularSet : public DiffuseSet {
public:
    /** excepcion si no se cumple la traduccion especifcamente en el triangulo (si es en el piso o invalido) */
    TriangularSet(const std::string& nombre, double a, double b, double c);

    double calcularPertenencia(double x) const override;
    std::string getTipo() const override;
    std::string getPuntosTexto() const override;
    double getLimiteInferior() const override;
    double getLimiteSuperior() const override;

    double getA() const {
        return a;
    }
    double getB() const {
        return b;
    }
    double getC() const {
        return c;
    }
    private:
    double a; // inicio del soporte
    double b; // pico del triangulo
    double c; // Fin del soporte
 };


#endif