#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <ctype.h>
#include <algorithm>

struct noHeap{
    char cElem;
    bool bRead;
    std::string sArquivo;
    int count;
    int length;

    noHeap(std::string s){
        sArquivo = s;
        cElem = '|';
        count = 0;
        bRead = true;
        length = -1;
        getElem();
    }
    void getElem(){
        if(bRead){
            std::ifstream fArquivo {sArquivo};
            if(length < 0){
                fArquivo.seekg(0, fArquivo.end);
                length = fArquivo.tellg();
            }
            fArquivo.seekg(count, fArquivo.beg);
            if(count < length){
                fArquivo >> cElem;
                count += 2;
            }else{
                cElem = '|';
                bRead = 0;
            }
            fArquivo.close();
        }
        else
            cElem = '|';
    }
};
struct Comparador{
    bool operator() (const noHeap & n1, const noHeap & n2){
        return int(n1.cElem) < int(n2.cElem);
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
    void auxClfInterna(std::vector<char> & part, int & nextFile, bool gSentinela){
        std::string sOutput; //Arquivo para gravar o vetor
        sOutput = "input" + std::to_string(nextFile) + ".txt"; // Na próxima fase serão os 1º inputs =/
        quickSort(part, 0, part.size() -1);
        std::ofstream fOutput(sOutput, std::ios_base::app);
        for(int j = 0, i = part.size(); j < i; j++)
            fOutput << part[j] << '\n';
       
        if(gSentinela)
            fOutput << Sentinela << '\n';

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
                        auxClfInterna(part, nextFile,true);
                        i=0;
                        part.clear();
                        processar = false;
                    }
                }
            }
            if(processar)
                auxClfInterna(part, nextFile, false);
        } else
            throw std::runtime_error{"Erro na abertura do arquivo de entrada!!"};
    }

    // Fase de intercalação seguindo o Modelo de N-Caminhos Balanceada
    // sName = Nome do Arquivo de Saída   
    void intercalacao(std::string sName){
        alternateIOS = true;
        std::string sOutput;
        //for(int N = P; N > 1; N = ceil(float(N/W))){
        int N = P;
        while(N > 1){
            for(int i = 0; i < N; i++){
                sOutput = (alternateIOS ? "output" : "input") + std::to_string(i % W) + ".txt";
                percorre_particoes(sOutput.c_str(), true);
            }
            alternateIOS = !alternateIOS;
            minHeap.clear();
            N = ceil((float)N/W);
        }
        percorre_particoes(sName.c_str(), false);
    }

    void clearFiles(){
        std::string sOutput;
        for(int i = 0; i < W; i++){
            sOutput = (alternateIOS ? "output" : "input") + std::to_string(i) + ".txt";
            std::remove(sOutput.c_str());
        }
    }

    void percorre_particoes(const char* sOutput, bool gSentinela){
        std::ofstream fOutput;
        if(minHeap.empty()){
            clearFiles();
            fOutput.open(sOutput);
            std::string sInput;
            //std::make_heap(minHeap.begin(),minHeap.end(),Comparador());
            for(int i = 0; i < W; i++){
                sInput = (alternateIOS ? "input" : "output") + std::to_string(i) + ".txt";
                noHeap no { sInput };
                minHeap.push_back(noHeap::noHeap(sInput));
                //std::push_heap(minHeap.begin(),minHeap.end(),Comparador());
            }
        } else {
            fOutput.open(sOutput, std::ios_base::app);
            for(auto & it : minHeap)
                it.getElem();
            //std::make_heap(minHeap.begin(),minHeap.end(),Comparador());
        };
        while(true){
            auto min = std::min_element(minHeap.begin(), minHeap.end(), Comparador());
            //noHeap & min = minHeap.front();
            if((*min).cElem == Sentinela)
                break;
            fOutput << (*min).cElem << '\n';
            (*min).getElem();
            //std::make_heap(minHeap.begin(),minHeap.end(),Comparador());
        }
        if(gSentinela)
            fOutput << Sentinela << '\n';

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
    exSorting ex{2,6};
    ex.clfInterna("input.txt");
    ex.intercalacao("output.txt");
    return 0;
}