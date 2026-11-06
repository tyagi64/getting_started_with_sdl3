#pragma  once
#include<SDL3/SDL.h>
#include<Circle.hpp>
#include<iostream>
#include <utility>
#include <vector>

class GDM {
    private:
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
        bool running = true;
        std::vector<std::pair<Circle, Circle>> circles;
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
            for(float i = 0 ;i <= 360;i+=0.5f){
                float angle = i * (M_PI/180.0f);
                float x = c.radius * std::sin(angle);
                float y = c.radius * std::cos(angle);
                createLine(lx+c.center.x,ly+c.center.y,x + c.center.x, y + c.center.y);
                // createLine(c.center.x,c.center.y,x+c.center.x,y + c.center.y);
                lx = x;
                ly = y;
            }
            
        }
        void render(){
            SDL_SetRenderDrawColor(renderer, 16, 24, 39, 255);
            SDL_RenderClear(renderer);
            SDL_SetRenderDrawColor(renderer, 56, 189, 248, 255);
            for(auto &c : circles){
                drawCircle(c.first);
                c.first.fall();
            }
            SDL_RenderPresent(renderer);
        }
        void pushCircle(Circle && c){
            circles.push_back({c,c});
        }
        void handleKeyPress(SDL_Scancode keyCode,SDL_Keymod mod){
            if(mod & SDL_KMOD_SHIFT){
                std::cout<<"SHIFT ACTIVE!! \n";
                std::cout<< "KEY Pressed "<<SDL_GetScancodeName(keyCode)<<std::endl;
                switch (keyCode) {
                case SDL_SCANCODE_Q:
                    cancel();
                    break;
                case SDL_SCANCODE_C:
                    for(auto &c : circles){
                        c.first = c.second;
                    }
                    break;
                default:
                    break;
                }
            }
        
            if(mod & SDL_KMOD_CTRL){
                std::cout<<"CTRL ACTIVE!! \n";
            }
        
            if(mod & SDL_KMOD_ALT){
                std::cout<<"ALT ACTIVE!! \n";
            }
        }
        void handleEvents(){
            while(running && SDL_PollEvent(&event)){
                switch (event.type) {
                    case SDL_EVENT_QUIT: 
                            cancel();
                            break;
                    case SDL_EVENT_KEY_DOWN:
                            handleKeyPress(event.key.scancode,event.key.mod);
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