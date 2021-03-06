#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void alu_control_unit_init(ALUControlUnit *acu, Bus *funct, Path *aluop, Path *op){
    //前ページの回路図のように回路を作成する
    Path *npath;
    npath = (Path *)malloc(sizeof(Path) * 2);

    Path *apath;
    apath = (Path *)malloc(sizeof(Path));

    Path *opath;
    opath = (Path *)malloc(sizeof(Path));

    acu->ogates = (ORGate *)malloc(sizeof(ORGate) * 3);
    acu->agates = (ANDGate *)malloc(sizeof(ANDGate) * 2);
    acu->ngates = (NOTGate *)malloc(sizeof(NOTGate) * 2);

    orgate_init(acu->ogates + 0, bus_get_path(funct, 3), bus_get_path(funct, 0), opath);
    orgate_init(acu->ogates + 1, aluop, apath, op + 2);
    orgate_init(acu->ogates + 2, npath, npath + 1, op + 1);

    andgate_init(acu->agates, aluop + 1, bus_get_path(funct, 1), apath);
    andgate_init(acu->agates + 1, aluop + 1, opath, op);

    notgate_init(acu->ngates + 0, aluop + 1, npath);
    notgate_init(acu->ngates + 1, bus_get_path(funct, 2), npath + 1);
}

void alu_control_unit_run(ALUControlUnit *acu) {
    //順番に気をつけて各ゲートをrunする
    andgate_run(acu->agates);
    orgate_run(acu->ogates + 1);
    notgate_run(acu->ngates);
    notgate_run(acu->ngates + 1);
    orgate_run(acu->ogates + 2);
    orgate_run(acu->ogates);
    andgate_run(acu->agates + 1);
    printf("\naluunit\n");
    printf("funct1 %d\n", path_get_signal(acu->agates->in2));
    printf("agate0 out %d\n", path_get_signal(acu->agates->out1));
    printf("aluOp1 %d\n", path_get_signal(acu->agates->in1));
    printf("acuout op[0]:%d op[1]:%d op[2]:%d\n", path_get_signal(acu->agates[1].out1), path_get_signal(acu->ogates[2].out1), path_get_signal(acu->ogates[1].out1));
}

void alu_control_unit_release(ALUControlUnit *acu) {
    free(acu->ngates->out1);
    free(acu->ngates); 
    free(acu->ngates[1].out1);
    free(acu->ngates + 1);    
    free(acu->ogates->out1);
    free(acu->agates->out1);

    free(acu->ogates);
    free(acu->ogates + 1);
    free(acu->ogates + 2);

    free(acu->agates);
    free(acu->agates + 1);    
}