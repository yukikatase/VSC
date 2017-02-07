#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void fa_init(FA *f, Path *in1, Path *in2, Path *carry_in, Path *out1, Path *carry_out){
    Path *inner1, *inner2, *inner3;
    inner1 = (Path *)malloc(sizeof(Path));
    inner2 = (Path *)malloc(sizeof(Path));
    inner3 = (Path *)malloc(sizeof(Path));

    path_init(inner1);
    path_init(inner2);
    path_init(inner3);

    f->xorc1 = (XORCircuit *)malloc(sizeof(XORCircuit));
    f->xorc2 = (XORCircuit *)malloc(sizeof(XORCircuit));
    f->agate1 = (ANDGate *)malloc(sizeof(ANDGate));
    f->agate2 = (ANDGate *)malloc(sizeof(ANDGate));
    f->ogate1 = (ORGate *)malloc(sizeof(ORGate));

    xorcircuit_init(f->xorc1, in1, in2, inner1);
    xorcircuit_init(f->xorc2, carry_in, inner1, out1);
    andgate_init(f->agate1, in1, in2, inner2);
    andgate_init(f->agate2, carry_in, inner1, inner3);
    orgate_init(f->ogate1, inner2, inner3, carry_out);
}

void fa_run(FA *f){
    xorcircuit_run(f->xorc1);
    xorcircuit_run(f->xorc2);
    andgate_run(f->agate1);
    andgate_run(f->agate2);
    orgate_run(f->ogate1);
}
void fa_release(FA *f){
    free(f->xorc1->ogate1->out1);
    xorcircuit_release(f->xorc1);
    xorcircuit_release(f->xorc2);
    free(f->agate1->out1);
    free(f->agate1);
    free(f->agate2->out1);
    free(f->agate2);
    free(f->ogate1);
}

void fa_driver(Signal a, Signal b, Signal carry_in){
    Path in1, in2, carryrin1, out1, carryout1;
    FA f;

    path_init(&in1);
    path_init(&in2);
    path_init(&carryrin1);
    path_init(&out1);
    path_init(&carryout1);

    path_set_signal(&in1, a);
    path_set_signal(&in2, b);
    path_set_signal(&carryrin1, carry_in);

    fa_init(&f, &in1, &in2, &carryrin1, &out1, &carryout1);
    fa_run(&f);

    printf("FA(%d, %d, %d) => (%d, %d)\n", a, b, carry_in, path_get_signal(fa_get_out_path(&f)) ,path_get_signal(fa_get_carry_out_path(&f)));

    fa_release(&f);
}

Path* fa_get_out_path(FA *f){
    return f->xorc2->ogate1->out1;
}

Path* fa_get_carry_out_path(FA *f){
    return f->ogate1->out1;
}