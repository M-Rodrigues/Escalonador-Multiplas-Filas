#include<bits/stdc++.h>
using namespace std;

// Controlador de operações de E/S
struct CtrlES {
    int remaining_es;       // Indica se ainda resta E/S a ser executada
    int t_es;               // Tempo padrão de 1 operação de E/S para todos os processos
    int cur_es;             // Tempo de E/S relativo ao processos que está em E/S neste momento
    Processo * em_es;       // Processo que está executando a operação de E/S neste momento
    list<Processo*> q;      // Fila de processos esperando para realizar sua operação de E/S

    CtrlES(): em_es(nullptr), cur_es(0), remaining_es(0) {}
    
    // Adiciona um processo na fila
    void push(Processo *p) { q.push_back(p); }
    
    // Diz se existe algum processo realizando E/S
    bool ociosa() { return em_es == nullptr; }

    // Pega o proximo processo da fila e o coloca para realizar sua operação de E/S
    void escalonar() {
        if (!q.empty()) {
            em_es = q.front();
            cur_es = 0;
            q.pop_front();
            remaining_es = 1;
        } 
    }

    // Diz se existe algum processo espearando para fazer E/S ou não 
    bool empty() { return q.empty(); }

    // Realiza 1ms de operação E/S para o processo em E/S neste momento. Atualiza os atributos
    Processo * executa() {  
        cur_es++;
        
        // Fim da operação de E/S
        if (cur_es == t_es) {
            Processo * aux = em_es;
            em_es = nullptr;
            cur_es = 0;
            return aux;
        }
        
        return nullptr; 
    }

    // Printa na tela informações sobre todos os processos na fila
    void show() {
        if (q.empty() and em_es == nullptr) {
            printf("E/S\t\tVazia!\n"); return;
        }

        if (em_es != nullptr) {
            printf(
                "Em E/S: %s\t(%d/%d)\n",
                em_es->name.c_str(),
                cur_es,
                t_es);
        }

        for (Processo *p : q)
            printf(
                "%s\t\t%d\n",
                p->name.c_str(), 
                p->n_es);
    }
};