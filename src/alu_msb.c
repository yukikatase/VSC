#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void alu_msb_init(ALU_MSB *alu_msb, Path* op, Path *a, Path *b, Path *less, Path *carryIn, Path *s, Path *carryOut, Path *set) {
// ALU_MSB内のFAとMUXを結ぶPathをsetに置き換える
    Path *path;
    path = (Path *)malloc(sizeof(Path) * 2);
    path_init(path); path_init(path + 1);

    Path *bin;
    bin = (Path *)malloc(sizeof(Path) * 2);

    alu_msb->agate1 = (ANDGate *)malloc(sizeof(ANDGate));
    andgate_init(alu_msb->agate1, a, bin + 1, path);

    alu_msb->ogate1 = (ORGate *)malloc(sizeof(ORGate));
    orgate_init(alu_msb->ogate1, a, bin + 1, path + 1);

    alu_msb->ngate1 = (NOTGate *)malloc(sizeof(NOTGate));
    notgate_init(alu_msb->ngate1, b, bin);

    alu_msb->binvert = (MUX *)malloc(sizeof(MUX));
    mux_init(alu_msb->binvert, op + 2, b, bin, bin + 1);

    alu_msb->f1 = (FA *)malloc(sizeof(FA));
    fa_init(alu_msb->f1, a, bin + 1, carryIn, set, carryOut);

    alu_msb->mux = (MUX4 *)malloc(sizeof(MUX4));
    mux4_init(alu_msb->mux, op, op + 1, path, path + 1, set, less, s);
}
void alu_msb_run(ALU_MSB *alu_msb) {
    notgate_run(alu_msb->ngate1);
    mux_run(alu_msb->binvert);
    andgate_run(alu_msb->agate1);
    orgate_run(alu_msb->ogate1);
    fa_run(alu_msb->f1);
    mux4_run(alu_msb->mux);
}
void alu_msb_release(ALU_MSB *alu_msb) {
    free(alu_msb->ngate1->out1);//bin
    free(alu_msb->ngate1);
    free(alu_msb->binvert->ogate1->out1);//bin + 1
    mux_release(alu_msb->binvert);
    free(alu_msb->agate1->out1);
    free(alu_msb->agate1);
    free(alu_msb->ogate1->out1);
    free(alu_msb->agate1);
    fa_release(alu_msb->f1);
    mux4_release(alu_msb->mux);
}