#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void control_unit_init(ControlUnit *cu, Bus *opcode, Path *regDst, Path *aluSrc, Path *memToReg, Path *regWrite, Path *memWrite, Path *memRead, Path *branch, Path *aluOp) {
    // opcodeをNOTGateで反転させたものを用意
    // 適宜ANDGateに通す
    // ORGateやDUPを用いながら出力Pathに接続
    Path *path_hanten;
    path_hanten = (Path *)malloc(sizeof(Path) * 6);
    int i;    
    for(i = 0; i < 6; i++){
        path_init(path_hanten + i);
    }

    cu->ngates = (NOTGate *)malloc(sizeof(NOTGate) * 6);

    for(i = 0; i < 6; i++){
        notgate_init(cu->ngates + i, bus_get_path(opcode, i), path_hanten + i);    
    }

    // //andgateの接続開始
    Path **p1;
    p1 = (Path **)malloc(sizeof(Path*) * 6);
    for(i = 0; i < 6; i++){
        p1[i] = path_hanten + i;
    }

    Path **p2;
    p2 = (Path **)malloc(sizeof(Path*) * 6);
    p2[0] = bus_get_path(opcode, 0);
    p2[1] = path_hanten + 1;
    p2[2] = path_hanten + 2;
    p2[3] = path_hanten + 3;
    p2[4] = bus_get_path(opcode, 4);
    p2[5] = bus_get_path(opcode, 5);


    Path **p3;
    p3 = (Path **)malloc(sizeof(Path*) *6);
    p3[0] = bus_get_path(opcode, 0);
    p3[1] = path_hanten + 1;
    p3[2] = bus_get_path(opcode, 2);
    p3[3] = path_hanten + 3;
    p3[4] = bus_get_path(opcode, 4);
    p3[5] = bus_get_path(opcode, 5);

    Path **p4;
    p4 = (Path **)malloc(sizeof(Path*) *6);
    p4[0] = bus_get_path(opcode, 0);
    p4[1] = path_hanten + 1;
    p4[2] = path_hanten + 2;
    p4[3] = path_hanten + 3;
    p4[4] = bus_get_path(opcode, 4);
    p4[5] = bus_get_path(opcode, 5);

    cu->agatens = (ANDGateN *)malloc(sizeof(ANDGateN) * 4);

    Path *and_out;
    and_out = (Path *)malloc(sizeof(Path) *4);

    andgaten_init(cu->agatens + 0, p1, and_out + 0, 6);
    andgaten_init(cu->agatens + 1, p2, and_out + 1, 6);
    andgaten_init(cu->agatens + 2, p3, and_out + 2, 6);
    andgaten_init(cu->agatens + 3, p4, and_out + 3, 6);


    cu->ogates = (ORGate *)malloc(sizeof(ORGate) * 2);
    orgate_init(cu->ogates, and_out + 1, and_out + 2, aluSrc);
    memToReg = and_out + 1;
    orgate_init(cu->ogates + 1, and_out, and_out + 1, regWrite);
    memRead = and_out + 1;
    memWrite = and_out + 2;

    cu->dups = (DUP *)malloc(sizeof(DUP) * 2);
    dup_init(cu->dups, and_out, regDst, aluOp + 1);
    dup_init(cu->dups + 1, and_out + 3, branch, aluOp);
}

void control_unit_run(ControlUnit *cu) {
    //各回路を実行
    int i;
    for(i = 0; i < 6; i++){
        notgate_run(cu->ngates + i);
    }

    for(i = 0; i < 4; i++){
        andgaten_run(cu->agatens + i);
     }

    for(i = 0; i < 2; i++){
        orgate_run(cu->ogates + i);
    }
    dup_run(cu->dups);
    dup_run(cu->dups + 1);    
}

void control_unit_release(ControlUnit *cu) {
    int i;
    NOTGate *a = cu->ngates;
    for(i = 0; i < 6; i++){
        free(a->out1);
        free(a);
        a++;
    }

    ANDGateN *b = cu->agatens;
    for(i = 0; i < 4; i++){
        free(b->ins);
        free(b->out1);
        free(b);
        b++;
    }

    free(cu->ogates);
    free(cu->ogates + 1);
}