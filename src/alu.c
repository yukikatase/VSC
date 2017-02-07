#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void alu_init(ALU *alu, Path* op, Path *a, Path *b, Path *less, Path *carryIn, Path *s, Path *carryOut) {
    Path *path;
    path = (Path *)malloc(sizeof(Path) * 3);
    path_init(path); path_init(path + 1); path_init(path + 2);

    Path *bin;
    bin = (Path *)malloc(sizeof(Path) * 2);

    alu->agate1 = (ANDGate *)malloc(sizeof(ANDGate));
    andgate_init(alu->agate1, a, bin + 1, path);

    alu->ogate1 = (ORGate *)malloc(sizeof(ORGate));
    orgate_init(alu->ogate1, a, bin + 1, path + 1);

    alu->ngate1 = (NOTGate *)malloc(sizeof(NOTGate));
    notgate_init(alu->ngate1, b, bin);

    alu->binvert = (MUX *)malloc(sizeof(MUX));
    mux_init(alu->binvert, op + 2, b, bin, bin + 1);

    alu->f1 = (FA *)malloc(sizeof(FA));
    fa_init(alu->f1, a, bin + 1, carryIn, path + 2, carryOut);

    alu->mux = (MUX4 *)malloc(sizeof(MUX4));
    mux4_init(alu->mux, op, op + 1, path, path + 1, path + 2, less, s);
}

void alu_run(ALU *alu) {
// 全ての演算を計算して、最後にマルチプレクサで選択する
// 演算の種類に応じてif文で分岐してはならない
// つまり、回路の全ての要素についてrun()を呼ぶ必要がある
    notgate_run(alu->ngate1);
    mux_run(alu->binvert);
    andgate_run(alu->agate1);
    orgate_run(alu->ogate1);
    fa_run(alu->f1);
    mux4_run(alu->mux);
}

void alu_driver(int an, int bn, int cn, int dn, int en){
    ALU alu;
    Path a, b, less, carryIn, s, carryOut;
    Path *op;
    op = (Path *)malloc(sizeof(Path) * 3);
    path_init(op); path_init(op + 1); path_init(op + 2); path_init(&a); path_init(&b); path_init(&less); path_init(&carryIn); path_init(&s); path_init(&carryOut);

    path_set_signal(&a, an); path_set_signal(&b, bn); path_set_signal(op, cn); path_set_signal(op + 1, dn); path_set_signal(op + 2, en);
    alu_init(&alu, op, &a, &b, &less, &carryIn, &s, &carryOut);

    alu_run(&alu);
    printf("(A,B,OP[0],OP[1],Bin) = (%d, %d, %d, %d, %d) => S = %d, CA = %d\n", path_get_signal(&a), path_get_signal(&b), path_get_signal(op), path_get_signal(op + 1), path_get_signal(op + 2), path_get_signal(&s), path_get_signal(&carryOut));

    alu_release(&alu);
}

void alu_release(ALU *alu) {
    free(alu->ngate1->out1);//bin
    free(alu->ngate1);
    free(alu->binvert->ogate1->out1);//bin + 1
    mux_release(alu->binvert);
    free(alu->agate1->out1);
    free(alu->agate1);
    free(alu->ogate1->out1);
    free(alu->agate1);
    free(fa_get_out_path(alu->f1));
    fa_release(alu->f1);
    mux4_release(alu->mux);
}