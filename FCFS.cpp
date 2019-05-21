#include<bits/stdc++.h>
using namespace std;

// Fila de processos Q1
struct FCFS {
    list<Processo*> q;     // Fila de processos do RR
    
    // Printa na tela informações sobre o processo
    void print_process(Processo * p) {
        printf(
            "%s\t\t(%d/%d)\t\t%d\t\t%d\n",
            p->name.c_str(), 
            p->cur_cpu, 
            p->cur_b_cpu, 
            p->n_es,
            p->wait);
    }

    // Adiciona um processo na fila
    void push(Processo * p) { q.push_back(p); }

    void increase_wait(){
        for (Processo *p : q) p->wait++;
    }

    // Printa na tela informações sobre todos os processos na fila
    void show() {
        if (q.empty()) {
            printf("Q1: FCFS\tVazia!\n"); return;
        }

        printf("Q1: FCFS\nNome\t\tBurst CPU\tE/S restantes\tT espera\n");
        for (Processo* p : q) this->print_process(p);
    }
    
    // Diz se existe algum processo nessa fila ou não
    bool empty() { return q.empty(); }

    // Retira e retorna o próximo processo a ser executado da fila
    Processo * next() {
        Processo * aux = q.front();
        q.pop_front();
        return aux;
    }
    
    Processo * front() {
        Processo * aux = q.front();
        return aux;
    }
    
    void pop(){
        q.pop_front();
    }

    void return_process(Processo* p) {
        q.push_front(p);
    }

};