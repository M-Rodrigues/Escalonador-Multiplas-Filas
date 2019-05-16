#include<bits/stdc++.h>
using namespace std;

struct Processo {
    int b_cpu, n_es, cur_cpu;
    string name;

    Processo(int b, int es, int id): 
        b_cpu(b), n_es(es), cur_cpu(0) 
    {
        name = "P"; name.push_back('0'+id);
    }

    bool has_es() { return n_es > 0; }
};

struct RR {
    int quantum;
    queue<Processo*> q;

    RR(int q): quantum(q) {}
    
    void print_process(Processo * p) {
        printf(
            "%s\t\t(%d/%d)\t\t%d\n",
            p->name.c_str(), 
            p->cur_cpu, 
            p->b_cpu, 
            p->n_es);
    }
    
    void push(Processo * p) { q.push(p); }

    void show() {
        if (q.empty()) {
            printf("Q0: RR\t\tVazia!\n"); return;
        }

        printf("Q0: RR\nNome\t\tBurst CPU\tE/S restantes\n");
        Processo *head = q.front();
        q.pop();
        this->print_process(head);
        q.push(head);
        while (head != q.front()) {
            Processo * aux = q.front();
            q.pop();
            this->print_process(aux);
            q.push(aux);
        }
    }

    bool empty() { return q.empty(); }

    Processo * next() {
        Processo * aux = q.front();
        q.pop();
        return aux;
    }
};

struct FCFS {
    queue<Processo*> q;
    
    void print_process(Processo * p) {
        printf(
            "%s\t\t(%d/%d)\t\t%d\n",
            p->name.c_str(), 
            p->cur_cpu, 
            p->b_cpu, 
            p->n_es);
    }

    void push(Processo * p) { q.push(p); }

    void show() {
        if (q.empty()) {
            printf("Q1: FCFS\t\tVazia!\n"); return;
        }

        printf("Q1: FCFS\nNome\t\tBurst CPU\tE/S restantes\n");
        Processo *head = q.front();
        q.pop();
        this->print_process(head);
        q.push(head);
        while (head != q.front()) {
            Processo * aux = q.front();
            q.pop();
            this->print_process(aux);
            q.push(aux);
        }
    }
    bool empty() { return q.empty(); }

    Processo * next() {
        Processo * aux = q.front();
        q.pop();
        return aux;
    }
};

struct CtrlES {
    int t_es, cur_es;
    Processo * em_es;
    queue<Processo*> q;

    CtrlES(): em_es(nullptr), cur_es(0) {}
    
    void push(Processo *p) { q.push(p); }
    bool ociosa() { return em_es == nullptr; }
    void escalonar() {
        if (!q.empty()) {
            em_es = q.front();
            cur_es = 1;
            q.pop();
        } 
    }
    bool empty() { return q.empty(); }
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

        if (!q.empty()){
            printf("E/S\nNome\t\tE/S restantes\n");
            Processo *head = q.front();
            q.pop();
            printf(
                "%s\t\t%d\n",
                head->name.c_str(), 
                head->n_es);
            q.push(head);
            while (head != q.front()) {
                Processo * aux = q.front();
                q.pop();

                printf(
                    "%s\t\t%d\n",
                    aux->name.c_str(), 
                    aux->n_es);

                q.push(aux);
            }
        }
    }
};


struct Escalonador {
    int t = 0;
    RR *q0;
    FCFS *q1;

    CtrlES * ctrlES;

    Processo * p_cpu = nullptr;
    int fila = -1;
    int cur_cpu = 0;

    void print_state() {
        printf("\n-------------------------------------\n");
        printf("T = %d\n",t);
        printf("CPU: %s\t(%d,%d)\tfila: %d\n",
            p_cpu == nullptr ? "-" : p_cpu->name.c_str(),
            p_cpu == nullptr ? 0 : p_cpu->cur_cpu,
            p_cpu == nullptr ? 0 : p_cpu->b_cpu,
            fila);
        printf("-------------------------------------\n");
        q0->show(); 
        printf("\n"); 
        q1->show();
        printf("\n"); 
        ctrlES->show();
        printf("-------------------------------------\n\n"); 
    }

    Escalonador(RR *q0, FCFS *q1): q0(q0), q1(q1) {
        ctrlES = new CtrlES(); ctrlES->t_es = 25;
    }
    void start() {
        while (1) {
            this->print_state();
            
            // Se nenhum processo em ES -> Puxar algum processo da fila de ES
            if (ctrlES->ociosa()) {
                ctrlES->escalonar();
            } else {
            // Se processo em ES
                // Se ainda tem ES -> mantem e executa
                Processo * p = ctrlES->executa();
                // Senao, Retorna processo para Q0:RR
                if (p != nullptr) { q0->push(p); p->n_es--; p->cur_cpu=0; }
            }

            // Se CPU não vazia
            if (p_cpu != nullptr) {
                // Executa 1 mseg
                p_cpu->cur_cpu++;
                // Se fim burst_cpu
                if (p_cpu->cur_cpu == p_cpu->b_cpu) {
                    // Se possui E/S -> fila de E/S
                    if (p_cpu->has_es()) {
                        ctrlES->push(p_cpu);
                    } else {
                    // Senao -> fim do processo
                        delete p_cpu;
                    }

                    p_cpu = nullptr;
                    fila = -1;
                    cur_cpu = 0;
                }
                else {
                // Senao
                    // Se processo de RR e fim do quantum -> vai pra FCFS
                    if (fila == 0 and p_cpu->cur_cpu == q0->quantum) {
                        q1->push(p_cpu);
                    
                        p_cpu = nullptr;
                        fila = -1;
                    }
                }
            }                 
            else {
            // Se CPU vazia -> escalonar algum processo
                // Se Q0:RR não vazia -> escalona de RR
                if (!q0->empty()) {
                    p_cpu = q0->next();
                    // p_cpu->cur_cpu = 1;
                    fila = 0;
                } else {
                // Senao, Se Q1:FCFS não vazia -> escalona de FCSF
                    if (!q1->empty()) {
                        p_cpu = q1->next();
                        p_cpu->cur_cpu=1;
                        fila = 1;
                    } else {
                        // Senao -> Fim da simulação
                        break;
                    }
                }
            }
            t++;

            if (t == 100) break;

            string s; cin >> s;
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


    escal->start();

    return 0;
}