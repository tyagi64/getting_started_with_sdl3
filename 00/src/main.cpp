#include<iostream>
#include<SDL3/SDL.h>
#include <vector>


class Point{
    public:
        float x,y;
        Point(float x_in, float y_in){
            x = x_in;
            y = y_in;
        }
};
class Object{
    public:
        std::vector<Point> obj;
};
void putPoints(SDL_Renderer* renderer,std::vector<Object>& objs){
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    
    for(Object obj : objs){
        std::vector<Point> ps = obj.obj;
        for (size_t i = 1; i < ps.size(); i++) {
                SDL_RenderLine(
                    renderer,
                    ps[i - 1].x,
                    ps[i - 1].y,
                    ps[i].x,
                    ps[i].y
                );
            }
    }
}
bool handleKeyPress(SDL_Renderer* renderer,std::vector<Object>& objs,SDL_Scancode keyCode,SDL_Keymod mod){
    bool output = true;
    if(mod & SDL_KMOD_SHIFT){
        std::cout<<"SHIFT ACTIVE!! \n";
        std::cout<< "KEY Pressed "<<SDL_GetScancodeName(keyCode)<<std::endl;
        switch (keyCode) {
        case SDL_SCANCODE_Q:
            output = false;
            break;
        case SDL_SCANCODE_C:
            objs.clear();
            objs.push_back(Object());
            SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
            break;
        case SDL_SCANCODE_Z:
            if(!objs.empty()){
                objs.pop_back();
                if (objs.empty()) {
                    objs.push_back(Object());
                }
                SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
                SDL_RenderClear(renderer);
                putPoints(renderer, objs);
                SDL_RenderPresent(renderer);
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
    return  output;
}

int main(){

    std::cout<<"SDL3 setup successful TODO: Code w roger"<<std::endl;
    if(!SDL_Init(SDL_INIT_VIDEO)){
        std::cout<< "SDL INIT Failed due to "<<SDL_GetError();
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Create a Line",1280, 720,SDL_WINDOW_RESIZABLE);

    if(nullptr == window){
        std::cout<< "Window creation failed "<<SDL_GetError();
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window,nullptr);
    if(nullptr == renderer){
        std::cout << "Renderer creation failed: "
                     << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Event loop
    bool running = true;
    SDL_Event event;
    std::vector<Object> objs;
    objs.push_back(Object());
    while (running) {
        while (running && SDL_PollEvent(&event)) {
            switch (event.type){
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    std::cout<< "KEY Pressed!!! "<<SDL_GetKeyName(event.key.key)<<std::endl;
                    if(handleKeyPress(renderer,objs,event.key.scancode, event.key.mod)){
                        std::cout<<"";
                    }else{
                        std::cout<<"QUITTING!!"<<std::endl;
                        running = false;
                    }
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    std::cout<<"MOUSE MOVED: \n";
                    if (event.motion.state & SDL_BUTTON_LMASK) {
                        objs.back().obj.push_back(Point(event.motion.x,event.motion.y));
                        putPoints(renderer, objs);
                        SDL_RenderPresent(renderer);
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                        objs.push_back(Object());
                    break;
                default:
                    break;
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
