#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void alu32_init(ALU32 *alu32, Path *op, Bus *a, Bus *b, Bus *s, Path *carryOut) {
// ALUを32個作成して、つなげる
    alu32->alus = (ALU *)malloc(sizeof(ALU) * 31);
    alu32->alumsb1 = (ALU_MSB *)malloc(sizeof(ALU_MSB));

    Path *less;
    less = (Path *)malloc(sizeof(Path) * 31);
    int i;
    for(i = 0; i < 32; i++){
        path_init(less + i);
    }

    Path *carry_out;
    carry_out = (Path *)malloc(sizeof(Path) * 31);
    for(i = 0; i < 31; i++){
        path_init(carry_out + i);
    }

    Path *set;
    set = (Path *)malloc(sizeof(Path));
    path_init(set);

    alu_init(alu32->alus, op, bus_get_path(a, 0), bus_get_path(b, 0), set, op + 2, bus_get_path(s, 0), carry_out);
    for(i = 1; i < 31; i++){
        alu_init(alu32->alus + i, op, bus_get_path(a, i), bus_get_path(b, i), less + i - 1, carry_out + i - 1, bus_get_path(s, i), carry_out + i);
    }
    alu_msb_init(alu32->alumsb1, op, bus_get_path(a, 31), bus_get_path(b, 31), less + 30, carry_out + 30, bus_get_path(s, 31), carryOut, set);
}

void alu32_run(ALU32 *alu32) {
// ALUを順番に実行
    int  i;
    for(i = 0; i < 31; i++){
        alu_run(alu32->alus + i);
    }
    alu_msb_run(alu32->alumsb1);

    for(i = 0; i < 31; i++){
        alu_run(alu32->alus + i);
    }
    alu_msb_run(alu32->alumsb1);
}

void alu32_driver(int an, int bn, int cn, int dn, int en){
    ALU32 alu32;
    Path *op;
    op = (Path *)malloc(sizeof(Path) * 3);
    Path carryOut;
    Bus a, b, s;
    path_init(op); path_init(op + 1); path_init(op + 2); path_init(&carryOut);
    bus_init(&a, 32); bus_init(&b, 32); bus_init(&s, 32);

    bus_set_value(&a, an); bus_set_value(&b, bn);
    path_set_signal(op, cn); path_set_signal(op + 1, dn); path_set_signal(op + 2, en);

    alu32_init(&alu32, op, &a, &b, &s, &carryOut);
    alu32_run(&alu32);

    printf("(A,B,OP[0],OP[1],Bin) = (%d, %d, %d, %d, %d) => S = %d\n", bus_get_value(&a), bus_get_value(&b), path_get_signal(op), path_get_signal(op + 1), path_get_signal(op + 2), bus_get_value(&s));

    alu32_release(&alu32);
}

void alu32_release(ALU32 *alu32) {
    int i;
    ALU *a = alu32->alus; a++;
    for(i = 1; i < 31; i++){
        free(a->f1->agate2->in1); free(a->mux->agatens->ins[2]); alu_release(a); a++;
    }
    free(alu32->alumsb1->f1->agate2->in1); free(alu32->alumsb1->mux->agatens->ins[2]); alu_msb_release(alu32->alumsb1);
}