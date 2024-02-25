#ifndef UE_H
#define UE_H

#include "LTE.h"

struct Point {
    double x, y;

    Point(double x, double y):
        x(x), y(y)
    {

    }

    Point(Point *point):
        x(point->x), y(point->y)
    {

    }
};

class UE
{
public:
    UE(Point position);

    double calculate_throughput();

private:
    Point m_position;
    double m_last_average_throughput;
};

#endif // UE_H
