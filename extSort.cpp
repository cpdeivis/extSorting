#include "extSort.hpp"
#include <math.h>
#include <ctype.h>
#include <algorithm>

// Definição de Funções da noHeap
noHeap::noHeap(std::string s){
    sArquivo = s;
    cElem = '|';
    count = 0;
    bRead = true;
    length = -1;
    getElem();
}
void noHeap::getElem(){
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
        } else{
            cElem = '|';
            bRead = 0;
        }
        fArquivo.close();
    } else
        cElem = '|';
}
// end noHeap

//Particição do QuickSort
int extSort::particao(std::vector<char> &vetor, int menor, int maior){
    char pivo = vetor[maior];
    int i = menor - 1; // Indice do menor elemento
    for (int j = menor; j <= maior - 1; j++){
        // Se o elemento atual é <= ao pivo
        if (vetor[j] <= pivo){
            i++; // Incrementa o indice do menor elemento
            std::swap(vetor[i], vetor[j]);
        }
    }
    std::swap(vetor[i + 1], vetor[maior]);
    return (i + 1);
}
//Implementação do QuickSort
void extSort::quickSort(std::vector<char> &vetor, int menor, int maior){
    if (menor < maior){
        int pI = particao(vetor, menor, maior);
        quickSort(vetor, menor, pI - 1);
        quickSort(vetor, pI + 1, maior);
    }
}

// Fase de Classificação da Ordenação Externa
void extSort::auxClfInterna(std::vector<char> &part, int &nextFile, bool gSentinela){
    std::string sOutput; //Arquivo para gravar o vetor
    sOutput = "input" + std::to_string(nextFile) + ".txt"; // Na próxima fase serão os 1º inputs =/
    quickSort(part, 0, part.size() - 1);
    std::ofstream fOutput(sOutput, std::ios_base::app);
    for (int j = 0, i = part.size(); j < i; j++)
        fOutput << part[j] << '\n';

    if (gSentinela)
        fOutput << Sentinela << '\n';

    fOutput.close();
    if (nextFile == 0)
        P++;
    nextFile = (nextFile + 1) % W;
}
// sName = Nome do Arquivo de Entrada
void extSort::clfInterna(std::string sName){
    std::ifstream fEntrada{sName};
    if (fEntrada.is_open()){
        std::vector<char> part;
        char cAtual;
        int nextFile = 0, i = 0; //serve para saber em qual arquivo gravar a
                                 // partição, numero de registros lidos
        bool processar = true;
        P = 0;
        while (fEntrada >> cAtual){
            if (isalpha(cAtual)){
                processar = true;
                part.push_back(cAtual);
                i++;
                if (i == M){
                    auxClfInterna(part, nextFile, true);
                    i = 0;
                    part.clear();
                    processar = false;
                }
            }
        }
        if (processar)
            auxClfInterna(part, nextFile, false);
    }
    else
        throw std::runtime_error{"Erro na abertura do arquivo de entrada!!"};
}

// Fase de intercalação seguindo o Modelo de N-Caminhos Balanceada
// sName = Nome do Arquivo de Saída
void extSort::intercalacao(std::string sName){
    alternateIOS = true;
    std::string sOutput;
    int N = P;
    while (N > 1){
        for (int i = 0; i < N; i++){
            sOutput = (alternateIOS ? "output" : "input") + std::to_string(i % W) + ".txt";
            percorre_particoes(sOutput.c_str(), true);
        }
        alternateIOS = !alternateIOS;
        minHeap.clear();
        N = ceil((float)N / W);
    }
    percorre_particoes(sName.c_str(), false);
    minHeap.clear();
    clearFiles(true);
}

void extSort::clearFiles(bool all){
    std::string sOutput;
    for (int i = 0; i < W; i++){
        sOutput = (alternateIOS ? "output" : "input") + std::to_string(i) + ".txt";
        std::remove(sOutput.c_str());
        if(all){
            sOutput = (alternateIOS ? "input" : "output") + std::to_string(i) + ".txt";
            std::remove(sOutput.c_str());
        }
    }
}

void extSort::percorre_particoes(const char *sOutput, bool gSentinela){
    std::ofstream fOutput;
    if (minHeap.empty()){
        clearFiles(false);
        fOutput.open(sOutput);
        std::string sInput;
        for (int i = 0; i < W; i++){
            sInput = (alternateIOS ? "input" : "output") + std::to_string(i) + ".txt";
            noHeap no { sInput };
            minHeap.push_back(no);
        }
    }
    else{
        fOutput.open(sOutput, std::ios_base::app);
        for (auto &it : minHeap)
            it.getElem();
    };
    while (true){
        auto min = std::min_element(minHeap.begin(), minHeap.end(), Comparador());
        if ((*min).cElem == Sentinela)
            break;
        fOutput << (*min).cElem << '\n';
        (*min).getElem();
    }
    if (gSentinela)
        fOutput << Sentinela << '\n';

    fOutput.close();
}

extSort::extSort(int Mem, int Files){
    Sentinela = '|';
    M = Mem;
    W = Files / 2;
}