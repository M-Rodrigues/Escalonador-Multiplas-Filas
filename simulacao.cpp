#include<bits/stdc++.h>

#include "Processo.cpp"
#include "RR.cpp"
#include "FCFS.cpp"
#include "CtrlES.cpp"
#include "GanttDiagram.cpp"
#include "Escalonador.cpp"

using namespace std;

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