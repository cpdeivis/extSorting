#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <ctype.h>
#include <algorithm>

struct noHeap{
    char cElem;
    bool bNeof;
    std::ifstream fArquivo;

    noHeap(std::string s){
        fArquivo.open(s);
        bNeof = fArquivo.is_open() && fArquivo.good();
        cElem = '|';
        getElem();
    }
    void getElem(){
        if(bNeof){
            cElem = fArquivo.get();
            if(!fArquivo.good()){
                bNeof = false;
                fArquivo.close();
            }
        }
    }
};
struct Comparador{
    bool operator() (const noHeap & n1, const noHeap & n2){
        return n1.cElem > n2.cElem;
    }
};

struct exSorting{
    int M; //Memória de Leitura
    //F = Número de Arquivos
    int W; //W = Caminhos F/2 para o merge balanceado de N(F/2) caminhos
    int P; //Total de Partições por arquivo
    char Sentinela; //Indica o fim de uma partição no arquivo
    bool alternateIOS;
    std::vector<noHeap> minHeap;

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

    // Fase de Classificação da Ordenação Externa
    void auxClfInterna(std::vector<char> & part, int & nextFile){
        std::string sOutput; //Arquivo para gravar o vetor
        sOutput = "input_" + std::to_string(nextFile) + ".txt"; // Na próxima fase serão os 1º inputs =/
        quickSort(part, 0, part.size() -1);
        std::ofstream fOutput(sOutput, std::ios_base::app);
        for(int j = 0, i = part.size(); j < i; j++)
            fOutput << part[j] << '\n';

        fOutput << Sentinela;
        fOutput.close();
        if(nextFile == 0)
            P++;
        nextFile = (nextFile + 1) % W;
    }
    // sName = Nome do Arquivo de Entrada
    void clfInterna(std::string sName){
        std::ifstream fEntrada{ sName };
        if(fEntrada.is_open()){
            std::vector<char> part;
            char cAtual;
            int nextFile = 0, i = 0; //serve para saber em qual arquivo gravar a 
                                    // partição, numero de registros lidos
            bool processar = true;
            P = 0;
            while(fEntrada >> cAtual){
                if (isalpha(cAtual)){
                    processar = true;
                    part.push_back(cAtual);
                    i++;
                    if(i == M){
                        auxClfInterna(part, nextFile);
                        i=0;
                        part.clear();
                        processar = false;
                    }
                }
            }
            if(processar)
                auxClfInterna(part, nextFile);
        } else
            throw std::runtime_error{"Erro na abertura do arquivo de entrada!!"};
    }

    // Fase de intercalação seguindo o Modelo de N-Caminhos Balanceada
    // sName = Nome do Arquivo de Saída   
    void intercalacao(std::string sName){
        alternateIOS = true;
        std::string sOutput;
        for(int N = P; N > 1; ceil(N/W)){
            for(int i = 0; i < N-1; i++){
                sOutput = (alternateIOS ? "output" : "input") + std::to_string(i % W) + ".txt";
                percorre_particoes(sOutput, true);
            }
            alternateIOS = !alternateIOS;
            minHeap.clear();
        }
        percorre_particoes(sName, false);
    }

    void percorre_particoes(std::string sOutput, bool gSentinela){
        if(minHeap.empty()){
            std::string sInput;
            std::make_heap(minHeap.begin(),minHeap.end(),Comparador());
            for(int i = 0; i < W; i++){
                sInput = (alternateIOS ? "input" : "output") + std::to_string(i) + ".txt";
                noHeap no { sInput };
                minHeap.push_back(no);
                std::push_heap(minHeap.begin(),minHeap.end(),Comparador());
            }
        } else {
            for(auto & it : minHeap)
                it.getElem();
            std::sort_heap(minHeap.begin(),minHeap.end(),Comparador());
        }
        std::ofstream fOutput(sOutput);
        while(true){
            noHeap & min = minHeap.front();
            if(min.cElem == Sentinela)
                break;
            fOutput << min.cElem << '\n';
            min.getElem();
            std::sort_heap(minHeap.begin(),minHeap.end(),Comparador());
        }
        if(gSentinela)
            fOutput << Sentinela;

        fOutput.close();
    }

    //Constructor
    exSorting(int Mem, int Files){
        Sentinela = '|';
        M = Mem;
        W = Files/2;
    }
};

int main(int argc, char **argv){
    exSorting ex{4,4};
    ex.clfInterna("input.txt");
    return 0;
}