#include<iostream>
#include<SDL3/SDL.h>
#include <GDM.hpp>



int main(){

    std::cout<<"SDL3 setup successful TODO: Code w roger"<<std::endl;

    std::unique_ptr<GDM> app = GDM::create("My Awesome SDL3 Game");
    if (!app) {
       std::cerr << "Initialization failed. Exiting game.\n";
       return 1; 
    }
  
    std::cout << "Game started successfully!\n";
    for(float l = 10;l<400;l+=40){
        app->pushCircle(Circle(10,Point(l,10)));
    }
    app->run();
    return 0;
}
