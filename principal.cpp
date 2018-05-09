#include <iostream>
#include "extSort.hpp"

int main(int argc, char **argv){
    try{
        extSort ex{2,6};
        ex.clfInterna("input.txt");
        ex.intercalacao("output.txt");
    } catch(std::exception& e) {
        std::cout << "ERRO: " << e.what() << std::endl;
    }
    return 0;
}