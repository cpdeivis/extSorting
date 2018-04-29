#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <ctype.h>

struct ExtSorting{
    int M; //M = Memória de Leitura
    int F; //F = Numero de Arquivos
    int P; //P = Numero de Partições
    std::string fName; // Arquivo de entrada 

    //Particição do QuickSort
    int particao(std::vector<char> & vetor, int menor, int maior){
        char pivo = vetor[maior];
        int i = menor - 1;  // Indice do menor elemento
        for (int j = menor; j <= maior-1; j++)
        {
            // Se o elemento atual é <= ao pivo
            if (vetor[j] <= pivo){
                i++;    // Incrementa o indice do menor elemento
                std::swap(vetor[i], vetor[j]);  
            }
        }
        std::swap(vetor[i + 1], vetor[maior]);
        return (i + 1);
    }
    //Implementação do QuickSort
    void quickSort(std::vector<char> & vetor, int menor, int maior){
        if(menor < maior){
            int pI = particao(vetor, menor, maior);
            quickSort(vetor, menor, pI - 1);
            quickSort(vetor, pI + 1, maior);
        }
    }

    //Classificação Interna
    void auxClfInterna(std::vector<char> * part){
        std::string sOutput;
        int file;
        quickSort((*part), 0, M-1);
        if(P < F){
            sOutput = "output" + std::to_string(P) + ".txt";
            std::ofstream fOutput(sOutput.c_str());
            for(int k = 0; k < (*part).size(); k++)
                fOutput << (*part)[k];
            fOutput << "\n";
            fOutput.close();
        } else{
            file = (F-1 <= 1) ? 1 : (P%(F-1) == 0 ? F-1 : P%(F-1)); // Acha em qual arquivo gravar a partição
            sOutput = "output" + std::to_string(file) + ".txt";
            std::ofstream fOutput(sOutput.c_str(), std::ios_base::app); 
            for(int k = 0; k < (*part).size(); k++)
                fOutput << (*part)[k];
            fOutput << "\n";
            fOutput.close();
        }
    }
    void clfInterna(){
        std::ifstream fEntrada {fName};
        if(fEntrada.is_open()){
            std::vector<char> part;
            char cAtual; // Leitura atual do arquivo
            bool processar = true;
            int i = 0;
            P = 1;
            while(fEntrada >> cAtual){
                if (isalpha(cAtual)){
                    processar = true;
                    part.push_back(cAtual);
                    i++;
                    if(i == M){
                        auxClfInterna(&part);
                        P++;
                        i=0;
                        part.erase(part.begin(), part.end());
                        processar = false;
                    }
                }
            }
            fEntrada.close();
            if(processar){
                auxClfInterna(&part);
            } else {
                P--;
            }
        }
    }
};


int main(int argc, char **argv){
    ExtSorting ex;
    ex.F = 3;
    ex.M = 4;
    ex.fName = "input.txt";
    ex.clfInterna();
    return 0;
}