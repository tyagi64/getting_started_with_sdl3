#pragma once

#include <cmath>
#include <cstdlib>
class Point{
    public:
        float x,y;
        Point(float x_in, float y_in){
            x = x_in;
            y = y_in;
        }
        float Distance(Point b){
            float dx = b.x-x;
            float dy = b.y-y;
            float dist = std::sqrt((dx*dx)+(dy*dy));
            return dist;
        }
};