#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void mux_init(MUX *m, Path *ctl, Path *in1, Path *in2, Path *out1){
    Path *path;
    path = (Path *)malloc(sizeof(Path) * 3);
    path_init(path); path_init(path + 1); path_init(path + 2);

    m->agate1 = (ANDGate *)malloc(sizeof(ANDGate));
    m->agate2 = (ANDGate *)malloc(sizeof(ANDGate));
    m->ogate1 = (ORGate *)malloc(sizeof(ORGate));
    m->ngate1 = (NOTGate *)malloc(sizeof(NOTGate));

    notgate_init(m->ngate1, ctl, path);
    andgate_init(m->agate1, path, in1, path + 1);
    andgate_init(m->agate2, ctl, in2, path + 2);
    orgate_init(m->ogate1, path + 1, path + 2, out1);
}

void mux_run(MUX *m) {
    notgate_run(m->ngate1);
    andgate_run(m->agate1);
    andgate_run(m->agate2);
    orgate_run(m->ogate1);
}

void mux_driver(int an, int bn, int cn){
    MUX m;
    Path ctl, in1, in2, out1;
    path_init(&ctl); path_init(&in1); path_init(&in2); path_init(&out1);

    path_set_signal(&ctl, an); path_set_signal(&in1, bn); path_set_signal(&in2, cn);

    mux_init(&m, &ctl, &in1, &in2, &out1);
    mux_run(&m);

    printf("Ctl = %d, in1 = %d, in2 = %d => %d\n", path_get_signal(&ctl), path_get_signal(&in1), path_get_signal(&in2), path_get_signal(&out1));
    mux_release(&m);
}

void mux_release(MUX *m) {
    free(m->ngate1->out1);
    free(m->agate1->out1);
    free(m->agate2->out1);
    free(m->agate1);
    free(m->agate2);
    free(m->ogate1);
    free(m->ngate1);
}