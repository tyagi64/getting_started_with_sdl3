#include <cmath>
#include <cstdlib>
#include<iostream>
#include<SDL3/SDL.h>
#include <memory>

class Point{
    public:
        float x,y;
        Point(float x_in, float y_in){
            x = x_in;
            y = y_in;
        }
        void scale(Point s){
            x += s.x;
            y += s.y;
        }
};

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

class GDM {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
        bool running = true;
        Circle testCirc = Circle(40,Point(500,40));
        GDM(SDL_Window* win, SDL_Renderer* ren) : window(win), renderer(ren) {}
    public:
        
        ~GDM() {
              if (renderer) SDL_DestroyRenderer(renderer);
              if (window) SDL_DestroyWindow(window);
              SDL_Quit();
              std::cout<<"Quitting!!\n";
        }
        
        static std::unique_ptr<GDM> create(const char* title){
            
            if(!SDL_Init(SDL_INIT_VIDEO)){
                std::cout<< "SDL INIT Failed due to "<<SDL_GetError();
                return  nullptr;
            }
            
            SDL_Window* win  = SDL_CreateWindow(title,1280, 720,SDL_WINDOW_RESIZABLE); 
            if(nullptr == win){
                std::cout<< "Window creation failed "<<SDL_GetError();
                SDL_Quit();
                return nullptr;
            }
            
            SDL_Renderer* ren = SDL_CreateRenderer(win,nullptr);
            if(nullptr == ren){
                std::cout << "Renderer creation failed: " << SDL_GetError() << '\n';
                SDL_DestroyWindow(win);
                SDL_Quit();
                return nullptr;
            }
            return std::unique_ptr<GDM>(new GDM(win, ren));
        }
        GDM(const GDM&) = delete;
        GDM& operator=(const GDM&) = delete;

        void cancel(){
            running = false;
        }
        void createLine(float x1,float y1,float x2,float y2){
                SDL_RenderLine(renderer, x1,y1,x2,y2);
        }
        void drawCircle(Circle c){
            float lx= c.radius * std::sin(0),ly = c.radius * std::cos(0);
            for(float i = 0 ;i <= 360;i+=0.1f){
                float angle = i * (M_PI/180.0f);
                float x = c.radius * std::sin(angle);
                float y = c.radius * std::cos(angle);
                createLine(lx+c.center.x,ly+c.center.y,x + c.center.x, y + c.center.y);
                createLine(c.center.x,c.center.y,x+c.center.x,y + c.center.y);
                lx = x;
                ly = y;
            }
            
        }
        void render(){
            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            drawCircle(testCirc);
            SDL_RenderPresent(renderer);
            testCirc.fall();
        }
        void handleEvents(){
            while(running && SDL_PollEvent(&event)){
                switch (event.type) {
                    case SDL_EVENT_QUIT: 
                            cancel();
                            break;
                } 
            }
        }
        void run(){
            while(running){
                render();
                handleEvents();
                SDL_Delay(16);
            }
        }
};

int main(){

    std::cout<<"SDL3 setup successful TODO: Code w roger"<<std::endl;

    std::unique_ptr<GDM> app = GDM::create("My Awesome SDL3 Game");
    if (!app) {
       std::cerr << "Initialization failed. Exiting game.\n";
       return 1; 
    }
  
    std::cout << "Game started successfully!\n";
    app->run();
    return 0; // 'game' goes
}
