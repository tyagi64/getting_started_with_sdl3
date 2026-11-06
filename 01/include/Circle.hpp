#pragma once

#include <Point.hpp>
class Circle{
    public:
        Point center;
        float radius;
        float velocityY = 0.0f;
        float gravity = 0.15f;
        float restitution = 0.60f;
        Circle(float r,Point p): center(p),radius(r){}
        void fall(){
            velocityY +=  gravity;
            center.y+=velocityY;
            if(center.y +  radius >= 720){
                center.y = 720-radius;
                velocityY = -velocityY * restitution;
            }
            if(center.y-radius <= 0){
                center.y = 0 + radius;
                velocityY = -velocityY;
            }
        }
};
