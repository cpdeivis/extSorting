#pragma once

#include <fstream>
#include <vector>
#include <string>

class noHeap{
    private:
        bool bRead;
        std::string sArquivo;
        int count;
        int length;

    public:
        char cElem;
        noHeap(std::string s);
        void getElem();
};

struct Comparador{
    bool operator() (const noHeap & n1, const noHeap & n2){
        return int(n1.cElem) < int(n2.cElem);
    }
};

class extSort{
    private:
        int M; //Memória de Leitura
        int W; //W = Caminhos F/2 para o merge balanceado de N(F/2) caminhos
        int P; //Total de Partições por arquivo
        char Sentinela; //Indica o fim de uma partição no arquivo
        bool alternateIOS; //Flag para alternar entre arquivos de entrada e saída
        std::vector<noHeap> minHeap; //Vetor para representação de Heap mínima

        int particao(std::vector<char> & vetor, int menor, int maior);
        void quickSort(std::vector<char> & vetor, int menor, int maior);
        void auxClfInterna(std::vector<char> & part, int & nextFile, bool gSentinela); 
        void clearFiles(bool all);
        void percorre_particoes(const char* sOutput, bool gSentinela);

    public:
        extSort(int Mem, int Files);
        void intercalacao(std::string sName);
        void clfInterna(std::string sName);
};