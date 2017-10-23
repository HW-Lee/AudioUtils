#ifndef __CURVE_FITTER_CPP__
#define __CURVE_FITTER_CPP__

#include "CurveFitter.h"
#include <stdint.h>

std::vector<double> QuadraticFitter::fit(Point2D<double> p1, Point2D<double> p2, Point2D<double> p3)
{
    std::vector<double> coeffs(3);
    const double det = (p1.x - p2.x)*(p1.x - p3.x)*(p2.x - p3.x);
    coeffs[2] =  (p2.x-p3.x)          *p1.y - (p1.x-p3.x)          *p2.y + (p1.x-p2.x)          *p3.y;
    coeffs[1] = -(p2.x*p2.x-p3.x*p3.x)*p1.y + (p1.x*p1.x-p3.x*p3.x)*p2.y - (p1.x*p1.x-p2.x*p2.x)*p3.y;
    coeffs[0] =  p2.x*p3.x*(p2.x-p3.x)*p1.y - p1.x*p3.x*(p1.x-p3.x)*p2.y + p1.x*p2.x*(p1.x-p2.x)*p3.y;
    for (uint32_t i = 0; i < 3; i++) {
        coeffs[i] /= det;
    }

    return coeffs;
}

#endif
