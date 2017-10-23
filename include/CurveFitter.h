#ifndef __CURVE_FITTER_H__
#define __CURVE_FITTER_H__

#include <vector>

template<class T>
class Point2D
{
public:
    Point2D();
    Point2D(T x, T y);
    ~Point2D();

    T x;
    T y;
};

template<class T>
Point2D<T>::Point2D()
{
    this->x = 0;
    this->y = 0;
}

template<class T>
Point2D<T>::Point2D(T x, T y)
{
    this->x = x;
    this->y = y;
}

template<class T>
Point2D<T>::~Point2D()
{
}

class QuadraticFitter
{
public:
    static std::vector<double> fit(Point2D<double> p1, Point2D<double> p2, Point2D<double> p3);
    static Point2D<double> peak(std::vector<double> coeffs);

};

#endif
