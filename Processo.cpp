#include<bits/stdc++.h>
using namespace std;

// Modelagem do Processo
struct Processo {
    int b_cpu;      // Tempo de Burst CPU do processo
    int cur_b_cpu;  // Tempo de Burst CPU que ainda resta para o processo executar
    int n_es;       // Número de operações de E/S do processo
    int cur_cpu;    // Variável auxiliar para manter controle de quanto do Burst CPU já foi executado
    string name;    // Nome do processo (P1, P2, P3, ...)
    int wait;       // tempo de espera do processo na fila q1

    Processo(int b, int es, int id): 
        b_cpu(b), n_es(es), cur_cpu(0), cur_b_cpu(b), wait(0) {
        name = "P"; name.push_back('0'+id);
    }

    // Diz se processo ainda possui alguma operação de E/S para fazer
    bool has_es() { return n_es > 0; }
};