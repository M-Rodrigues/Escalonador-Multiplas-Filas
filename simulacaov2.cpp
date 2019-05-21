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

struct GanttDiagram {
    struct GanttItem {
        string process_name;
        int t_entrada;
        int t_saida;

        GanttItem(Processo* p, int t_ent):
            process_name(p->name),
            t_entrada(t_ent),
            t_saida(-1) {}
    };

    list<GanttItem*> l;
    
    void add_entrada_cpu(Processo* p, int t) {
        l.push_back(new GanttItem(p, t));
    }

    void add_saida_cpu(int t) {
        l.back()->t_saida = t;
    }

    void show() {
        printf("Diagrama de Gantt\n");
        for (GanttItem* it : l) this->print_item(it);
    }

    void print_item(GanttItem* it) {
        printf(
            "%s\t%d\t%d\n",
            it->process_name.c_str(),
            it->t_entrada,
            it->t_saida
            );
    }
};

struct Escalonador {
    int t = 0;                      // Contador de tempo global
    RR *q0;                         // Fila com maior prioridade
    FCFS *q1;                       // Fila com menor prioridade

    CtrlES * ctrlES;                // Controlador de E/S

    GanttDiagram * gantt;           // Diagrama de Gantt dos processos na CPU

    Processo * p_cpu = nullptr;     // Processo que está sendo executado na CPU
    int fila = -1;                  // flag indicando de qual fila é o processo que está na CPU
    int cur_cpu = 0;                // Quantos ms o processo que está na CPU já executou até o momento

    // Printa na tela todas as informações sobre Q0,Q1, CtrlES e CPU
    void print_state() {
        printf("\n--------------------------------------------------------------\n");
        printf("T = %d\n",t);
        printf("CPU: %s\t(%d,%d)\tfila: %d\n",
            p_cpu == nullptr ? "-" : p_cpu->name.c_str(),
            p_cpu == nullptr ? 0 : p_cpu->cur_cpu,
            p_cpu == nullptr ? 0 : p_cpu->cur_b_cpu,
            fila);
        printf("\n");
        q0->show(); 
        printf("\n"); 
        q1->show();
        printf("\n"); 
        ctrlES->show();
        printf("--------------------------------------------------------------\n\n"); 
    }

    void show_gantt_diagram() {
        gantt->show();
    }

    Escalonador(RR *q0, FCFS *q1): q0(q0), q1(q1), fila(-1) {
        ctrlES = new CtrlES(); ctrlES->t_es = 20;
        gantt = new GanttDiagram();
    }

    void pre_processing() {
        // Escalona processo na CPU
        if (p_cpu == nullptr) {
            if (!q0->empty()) {
                p_cpu = q0->next();
                fila = 0;
                gantt->add_entrada_cpu(p_cpu, t);
            } else if (!q1->empty()) {
                p_cpu = q1->next();
                fila = 1;
                p_cpu->cur_cpu = 0;
                
                gantt->add_entrada_cpu(p_cpu, t);
            }
        } else {
            // Realiza preempção
            if (fila == 1 and !q0->empty()) {
                // Volta pra q1
                q1->return_process(p_cpu);
                gantt->add_saida_cpu(t);

                // Escalona de q0
                p_cpu = q0->next();
                gantt->add_entrada_cpu(p_cpu, t);
            }
        }

        // Escalona para E/S
        if (ctrlES->ociosa()) ctrlES->escalonar();
    }

    void execute() {
        // Executa CPU
        p_cpu->cur_cpu++;

        // Executa E/S
        if (!ctrlES->ociosa()) {
            Processo * p = ctrlES->executa();
            ctrlES->remaining_es = 1;

            // Senao, Retorna processo para Q0:RR
            if (p != nullptr) {  //o processo p terminou a ES
                q0->push(p); 
                p->n_es--; 
                p->cur_cpu = 0; 
                p->cur_b_cpu = p->b_cpu; 
                ctrlES->remaining_es = 0; 
            }
        }

        // Aumenta tempo de espera dos processos na fila Q1
        q1->increase_wait();
    }

    void post_processing() {
        // tira da CPU
        if (p_cpu->cur_cpu == p_cpu->cur_b_cpu) {
            // para E/S
            if (p_cpu->has_es()) {
                ctrlES->push(p_cpu);
            } 
            // para FIM DO PROCESSO
            else {
                delete p_cpu;
            }

            p_cpu = nullptr;
            fila = -1;
            cur_cpu = 0;

            gantt->add_saida_cpu(t);
        } else {
            // para fila Q1
            if (fila == 0 and p_cpu->cur_cpu == q0->quantum) {
                q1->push(p_cpu);
                p_cpu->cur_b_cpu -= q0->quantum;
                p_cpu->wait = 0;
            
                p_cpu = nullptr;
                fila = -1;
                
                gantt->add_saida_cpu(t);
            }
        }

        // tira da E/S para q0
        // ---> Já foi feito em execute()

        // tira de q1 para q0 (envelhecimento)
        if(!q1->empty()){
            Processo *p = q1->front();
            if(p->wait == 25){
                //  o processo da frente da fila q1 chegou ao tempo maximo 
                // de espera entao esse processo deve ir para a fila q0 
                
                p->wait = 0;
                q1->pop();
                q0->push(p);
                p->cur_cpu = 0;
            }
        }
    }

    bool has_finished() {
        return 
            p_cpu == nullptr
            and q0->empty()
            and q1->empty()
            and ctrlES->ociosa()
            and ctrlES->empty();
    }

    void begin() {
        t = 0;
        this->print_state();
        while (!this->has_finished()) {
            this->pre_processing();
            this->execute();
            this->post_processing();
            t++;    
        
            this->print_state();
        }
    }

};

int main() {
    // Criando filas de processos
    RR *q0 = new RR(10);
    FCFS *q1 = new FCFS();
    Escalonador *escal = new Escalonador(q0,q1);

    // Lendo dados de entrada sobre os processos
    // Inicialmente, todos processos estão em Q0
    int n_processos; cin >> n_processos;
    for (int i = 1; i <= n_processos; i++) {
        int burst_cpu, n_es;
        cin >> burst_cpu >> n_es;

        Processo *p = new Processo(burst_cpu, n_es, i);
        q0->push(p);
    }

    escal->begin();

    escal->show_gantt_diagram();

    return 0;
}