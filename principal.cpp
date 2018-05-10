#include <iostream>
#include "extSort.hpp"

int main(int argc, char **argv){
    try{
        if(argc == 5){
            extSort ex{std::stoi(argv[1]),std::stoi(argv[2])};
            ex.clfInterna(argv[3]);
            ex.intercalacao(argv[4]);
        }
    } catch(std::exception& e) {
        std::cout << "ERRO: " << e.what() << std::endl;
    }
    return 0;
}
