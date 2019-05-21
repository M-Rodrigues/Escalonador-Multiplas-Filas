#include<bits/stdc++.h>
using namespace std;

// Modelagem do escalonador
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
                gantt->add_saida_cpu(t-1);
                p_cpu->cur_b_cpu -= (t - gantt->entrada_cpu());
                p_cpu->cur_cpu=0;

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
        if(p_cpu!=nullptr) p_cpu->cur_cpu++;

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
        if(p_cpu!=nullptr){
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
            and ctrlES->empty()
            and ctrlES->remaining_es==0;
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