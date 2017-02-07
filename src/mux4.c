#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void mux4_init(MUX4 *m4, Path *ctl1, Path *ctl2, Path *in1, Path *in2, Path *in3, Path *in4, Path *out1){
    Path *pa1;
    pa1 = (Path *)malloc(sizeof(Path) * 4);
    path_init(pa1); path_init(pa1 + 1); path_init(pa1 + 2); path_init(pa1 + 3);

    Path **path1; Path **path2; Path **path3; Path **path4; Path **path5;

    Path *pa2;
    pa2 = (Path *)malloc(sizeof(Path) * 4);
    path_init(pa2); path_init(pa2 + 1); path_init(pa2 + 2); path_init(pa2 + 3);

    path1 = (Path **)malloc(sizeof(Path*) * 3); path1[0] = pa1; path1[1] = pa1 + 1; path1[2] = in1;
    path2 = (Path **)malloc(sizeof(Path*) * 3); path2[0] = ctl1; path2[1] = pa1 + 2; path2[2] = in2;
    path3 = (Path **)malloc(sizeof(Path*) * 3); path3[0] = pa1 + 3; path3[1] = ctl2; path3[2] = in3;
    path4 = (Path **)malloc(sizeof(Path*) * 3); path4[0] = ctl1; path4[1] = ctl2; path4[2] = in4;

    path5 = (Path **)malloc(sizeof(Path*) * 4); path5[0] = pa2; path5[1] = pa2 + 1; path5[2] = pa2 + 2; path5[3] = pa2 + 3;

    m4->agatens = (ANDGateN *)malloc(sizeof(ANDGateN) * 4);
    andgaten_init(m4->agatens, path1, pa2, 3);
    andgaten_init(m4->agatens + 1, path2, pa2 + 1, 3);
    andgaten_init(m4->agatens + 2, path3, pa2 + 2, 3);
    andgaten_init(m4->agatens + 3, path4, pa2 + 3, 3);

    m4->ogatens = (ORGateN *)malloc(sizeof(ORGateN));
    orgaten_init(m4->ogatens, path5, out1, 4);

    m4->ngates = (NOTGate *)malloc(sizeof(NOTGate) * 4);
    notgate_init(m4->ngates, ctl1, pa1);
    notgate_init(m4->ngates + 1, ctl2, pa1 + 1);
    notgate_init(m4->ngates + 2, ctl2, pa1 + 2);
    notgate_init(m4->ngates + 3, ctl1, pa1 + 3);
}

void mux4_run(MUX4 *m4) {
    notgate_run(m4->ngates);
    notgate_run(m4->ngates + 1);
    notgate_run(m4->ngates + 2);
    notgate_run(m4->ngates + 3);

    andgaten_run(m4->agatens);
    andgaten_run(m4->agatens + 1);
    andgaten_run(m4->agatens + 2);
    andgaten_run(m4->agatens + 3);

    orgaten_run(m4->ogatens);
}

void mux4_driver(int an, int bn, int cn, int dn, int en, int fn){
    MUX4 m4;
    Path ctl1, ctl2, in1, in2, in3, in4, out1;
    path_init(&ctl1); path_init(&ctl2);
    path_init(&in1); path_init(&in2); path_init(&in3); path_init(&in4); path_init(&out1);

    path_set_signal(&ctl1, an); path_set_signal(&ctl2, bn);
    path_set_signal(&in1, cn); path_set_signal(&in2, dn); path_set_signal(&in3, en); path_set_signal(&in4, fn);

    mux4_init(&m4, &ctl1, &ctl2, &in1, &in2, &in3, &in4, &out1);
    mux4_run(&m4);

    ANDGateN *a = m4.agatens;
    printf("agate1 %d ", path_get_signal(a->out1));a++;
    printf("agate2 %d ", path_get_signal(a->out1));a++;
    printf("agate3 %d ", path_get_signal(a->out1));a++;
    printf("agate4 %d\n", path_get_signal(a->out1));

    printf("out = %d\n", path_get_signal(&out1));

    mux4_release(&m4);
}

void mux4_release(MUX4 *m4) {
    ANDGateN *a;
    a = m4->agatens;

    free(a->ins); free(a); a++;
    free(a->ins); free(a); a++;
    free(a->ins); free(a); a++;
    free(a->ins); free(a);

    NOTGate *b;
    b = m4->ngates;

    free(b->out1); free(b); b++;
    free(b->out1); free(b); b++;
    free(b->out1); free(b); b++;
    free(b->out1); free(b);

    free(m4->ogatens->ins);
    free(m4->ogatens);
}