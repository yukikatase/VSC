#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void clu_init(CLU *clu, Bus *g, Bus *p, Bus *c, Path *carry_out) {
    Path *inner;
    inner = (Path *)malloc(sizeof(Path) * 16);
    clu->agates = (ANDGate *)malloc(sizeof(ANDGate) * 10);
    clu->ogates = (ORGate *)malloc(sizeof(ORGate) * 10);
    // 回路を作成

    andgate_init(clu->agates, bus_get_path(p, 0), bus_get_path(c, 0), inner);
    andgate_init(clu->agates + 1, inner, bus_get_path(p, 1), inner + 1);
    andgate_init(clu->agates + 2, inner + 1, bus_get_path(p, 2), inner + 2);
    andgate_init(clu->agates + 3, inner + 2, bus_get_path(p, 3), inner + 3);

    andgate_init(clu->agates + 4, bus_get_path(g, 0), bus_get_path(p, 1), inner + 4);
    andgate_init(clu->agates + 5, inner + 4, bus_get_path(p, 2), inner + 5);
    andgate_init(clu->agates + 6, inner + 5, bus_get_path(p, 3), inner + 6);

    andgate_init(clu->agates + 7, bus_get_path(g, 1), bus_get_path(p, 2), inner + 7);
    andgate_init(clu->agates + 8, inner + 7, bus_get_path(p, 3), inner + 8);

    andgate_init(clu->agates + 9, bus_get_path(g, 2), bus_get_path(p, 3), inner + 8);


    orgate_init(clu->ogates, inner, bus_get_path(g, 0), bus_get_path(c, 1));

    orgate_init(clu->ogates + 1, inner + 1, inner + 4, inner + 10);
    orgate_init(clu->ogates + 2, inner + 10, bus_get_path(g, 1), bus_get_path(c, 2));

    orgate_init(clu->ogates + 3, inner + 2, inner + 5, inner + 11);
    orgate_init(clu->ogates + 4, inner + 11, inner + 7, inner + 12);
    orgate_init(clu->ogates + 5, inner + 12, bus_get_path(g, 2), bus_get_path(c, 3));

    orgate_init(clu->ogates + 6, inner + 3, inner + 6, inner + 13);
    orgate_init(clu->ogates + 7, inner + 13, inner + 8, inner + 14);
    orgate_init(clu->ogates + 8, inner + 14, inner + 9, inner + 15);
    orgate_init(clu->ogates + 9, inner + 15, bus_get_path(g, 3), bus_get_path(c, 4));
    carry_out = bus_get_path(c, 4);
}

void clu_run(CLU *clu) {
    int i;
    for(i = 0; i < 10; i++){
        andgate_run(clu->agates + i);
    }
    for(i = 0; i < 10; i++){
        orgate_run(clu->ogates + i);
    }
}

void clu_release(CLU *clu) {
    //確保した領域を解放
    ANDGate *a = clu->agates;
    ORGate *b = clu->ogates;
    int i;
    for(i = 0; i < 10; i++){
        free(a->out1);
        a++;
    }
    free(clu->agates);
    for(i = 11; i < 19; i++){
        free(b->out1);
        b++;
    }
    free(clu->ogates);
}

void clu_driver(int an, int bn, int cn){
    CLU clu;
    Bus g, p, c;
    Path carry_out;

    bus_init(&g, 4); bus_init(&p, 4); bus_init(&c, 5);
    path_init(&carry_out);

    clu_init(&clu, &g, &p, &c, &carry_out);
    bus_set_value(&g, an); bus_set_value(&p, bn); bus_set_value(&c, cn);

    clu_run(&clu);
    printf("CLU(g: %d, p: %d, carry_in: %d) => (%d)\n", an, bn, cn, bus_get_value(&c));

    clu_release(&clu);
}