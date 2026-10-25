#ifndef TRAPEZOIDAL_SET_H
#define TRAPEZOIDAL_SET_H

#include "DiffuseSet.h"

// Conjunto difuso con funcion de pertenencia trapezoidal que es definida por cuatro puntos

class TrapezoidalSet: public DiffuseSet {
    public:
    /** salta una excepcion si no se cumple a <= b <= c <= d, o si a == d */
    TrapezoidalSet(const std::string& nombre, double a, double b, double c, double d);

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
    double getD() const {
        return d;
    }

private:
    double a; // inicio del trapecio
    double b; // inicio del hombro del trapecii
    double c; //fin del hombro del trapecio
    double d; // fin del trapecio
};

#endif