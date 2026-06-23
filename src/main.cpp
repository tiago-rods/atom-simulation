#include "app/Application.h"
#include <iostream>

int main(){
    try {
        Application app(1280, 720, "Hidrogen Atom Simulation");
        app.run();
    } catch (const std::exception& e){
        std::cerr << "Fatal: " << e.what() << "\n";
        return 1;
    }
    return 0;
}