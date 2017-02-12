#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void pc_init(PC *pc, Bus *npaddr, Bus *paddr) {
    Path *wctl;
    wctl = (Path *)malloc(sizeof(Path));
    path_init(wctl);
    path_set_signal(wctl, true);
    pc->reg = (Register *)malloc(sizeof(Register));
    register_init(pc->reg, wctl, npaddr, paddr);
}

//初期アドレスをセット
void pc_set_value(PC *pc, int addr) {
    register_set_value(pc->reg, addr);
}

void pc_run(PC *pc) {
    register_run(pc->reg);
}

void pc_release(PC *pc) {
    free(pc->reg->wctl);
    free(pc->reg);
}