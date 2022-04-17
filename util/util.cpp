//
// Created by Jerry Ye on 2022/3/24.
//

#include "util.h"


double Util::D2Line::operator()(double x, double y) const
{
    return this->A * x + this->B * y + this->C;
}

double Util::D2Line::operator()(const Vec<double, 4>& p) const
{
    return this->operator()(p.getX(), p.getY());
}

double Util::D2Line::operator()(int x, int y) const
{
    return this->operator()(
            static_cast<double>(x),
            static_cast<double>(y)
            );
}

