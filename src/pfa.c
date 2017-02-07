#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void pfa_init(PFA *pfa, Path *a, Path *b, Path *carry_in, Path *g, Path *p, Path *s) {
// 各GateとFAの領域を確保
    pfa->agate1 = (ANDGate *)malloc(sizeof(ANDGate));
    pfa->ogate1 = (ORGate *)malloc(sizeof(ORGate));
    pfa->f1 = (FA *)malloc(sizeof(FA));

    Path *carry_out;
    carry_out = (Path *)malloc(sizeof(Path));
    path_init(carry_out);
// 回路を作成
    andgate_init(pfa->agate1, a, b, g);
    orgate_init(pfa->ogate1, a, b, p);
    fa_init(pfa->f1, a, b, carry_in, s, carry_out);
}

void pfa_run(PFA *pfa) {
// 各GateとFAを実行する
    andgate_run(pfa->agate1);
    orgate_run(pfa->ogate1);
    fa_run(pfa->f1);
}

void pfa_release(PFA *pfa) {
// initで確保された領域などを解放
    free(pfa->agate1);
    free(pfa->ogate1);
    free(fa_get_carry_out_path(pfa->f1));
    fa_release(pfa->f1);
    free(pfa->f1);
}

void pfa_driver(int an, int bn){
    PFA pfa;
    Path a, b, carry_in, g, p, s;
    path_init(&a); path_init(&b); path_init(&carry_in);
    path_init(&g); path_init(&p); path_init(&s);
    path_set_signal(&a, an);path_set_signal(&b, bn);

    pfa_init(&pfa, &a, &b, &carry_in, &g, &p, &s);
    pfa_run(&pfa);

    printf("PFA(%d, %d) carry_in = 0 => S: %d, carry_out: %d, g: %d, p: %d\n", an, bn, path_get_signal(fa_get_out_path(pfa.f1)), path_get_signal(fa_get_carry_out_path(pfa.f1)),
        path_get_signal(&g), path_get_signal(&p));

    pfa_release(&pfa);
}