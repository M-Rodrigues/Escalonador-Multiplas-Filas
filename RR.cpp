#include<bits/stdc++.h>

using namespace std;

// Fila de processos Q0
struct RR {
    int quantum;            // Quantum do RR
    list<Processo*> q;     // Fila de processos do RR

    RR(int q): quantum(q) {}
    
    // Printa na tela informações sobre o processo
    void print_process(Processo * p) {
        printf(
            "%s\t\t(%d/%d)\t\t%d\n",
            p->name.c_str(), 
            p->cur_cpu, 
            p->cur_b_cpu, 
            p->n_es);
    }
    
    // Adiciona um processo na fila
    void push(Processo * p) { q.push_back(p); }

    // Printa na tela informações sobre todos os processos na fila
    void show() {
        if (q.empty()) {
            printf("Q0: RR\t\tVazia!\n"); return;
        }

        printf("Q0: RR\nNome\t\tBurst CPU\tE/S restantes\n");
        for (Processo *p : q) this->print_process(p);
    }

    // Diz se existe algum processo nessa fila ou não
    bool empty() { return q.empty(); }

    // Retira e retorna o próximo processo a ser executado da fila
    Processo * next() {
        Processo * aux = q.front();
        q.pop_front();
        return aux;
    }
};