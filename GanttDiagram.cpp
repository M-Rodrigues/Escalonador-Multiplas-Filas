#include<bits/stdc++.h>
using namespace std;

// Diagrama de Gantt
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

    int entrada_cpu(){
        return l.back()-> t_entrada;
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