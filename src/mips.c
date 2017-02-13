#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void mips_init(MIPS *mips, int inst) {
    // ---回路作成---
    // 各種必要なPathやBusを初期化
    // 最低限必要なBusは
    // MIPS構造体内の各要素の領域を確保し、initを行う
    mips->pc = (PC *)malloc(sizeof(PC));
    Bus *pcin, *pcout;
    pcin = (Bus *)malloc(sizeof(Bus)), bus_init(pcin, 32);
    pcout = (Bus *)malloc(sizeof(Bus)), bus_init(pcout, 32);
    pc_init(mips->pc, pcin, pcout);

    mips->im = (InstMemory *)malloc(sizeof(InstMemory));
    Bus *imout;
    imout = (Bus *)malloc(sizeof(Bus));
    bus_init(imout, 32);
    inst_memory_init(mips->im, pcout, imout);

    mips->sb = (SplitBus *)malloc(sizeof(SplitBus));
    Bus *sb05, *sb1115, *sb1620, *sb2125, *sb2631;
    sb05 = (Bus *)malloc(sizeof(Bus));
    sb1115 = (Bus *)malloc(sizeof(Bus));
    sb1620 = (Bus *)malloc(sizeof(Bus));
    sb2125 = (Bus *)malloc(sizeof(Bus));
    sb2631 = (Bus *)malloc(sizeof(Bus));
    bus_init(sb05, 6), bus_init(sb1115, 5), bus_init(sb1620, 5), bus_init(sb2125, 5), bus_init(sb2631, 6);
    split_bus_init(mips->sb, imout, sb05, sb1115, sb1620, sb2125, sb2631);

    mips->cu = (ControlUnit *)malloc(sizeof(ControlUnit));
    Path *regDst, *aluSrc, *memToReg, *regWrite, *memWrite, *memRead, *branch, *aluOp;
    regDst = (Path *)malloc(sizeof(Path));
    aluSrc = (Path *)malloc(sizeof(Path));
    memToReg = (Path *)malloc(sizeof(Path));
    regWrite = (Path *)malloc(sizeof(Path));
    memWrite = (Path *)malloc(sizeof(Path));
    memRead = (Path *)malloc(sizeof(Path));
    branch = (Path *)malloc(sizeof(Path));
    aluOp = (Path *)malloc(sizeof(Path) * 2);
    path_init(regDst), path_init(aluSrc), path_init(memToReg), path_init(regWrite), path_init(memWrite);
    path_init(memRead), path_init(branch), path_init(aluOp), path_init(aluOp + 1);
    control_unit_init(mips->cu, sb2631, regDst, aluSrc, memToReg, regWrite, memWrite, memRead, branch, aluOp);


    mips->acu = (ALUControlUnit *)malloc(sizeof(ALUControlUnit));
    Path *acuout;
    acuout = (Path *)malloc(sizeof(Path) * 3);
    path_init(acuout);
    alu_control_unit_init(mips->acu, sb05, aluOp, acuout);

    mips->regfile = (RegisterFile *)malloc(sizeof(RegisterFile));
    Bus *rdata1, *rdata2, *wdata;
    rdata1 = (Bus *)malloc(sizeof(Bus)), bus_init(rdata1, 32);
    rdata2 = (Bus *)malloc(sizeof(Bus)); bus_init(rdata2, 32);
    wdata = (Bus *)malloc(sizeof(Bus)); bus_init(wdata, 32);
    registerfile_init(mips->regfile, regWrite, sb2125, sb1620, sb1115, wdata, rdata1, rdata2);

    mips->alu = (ALU32 *)malloc(sizeof(ALU32));
    Path *carryout;
    carryout = (Path *)malloc(sizeof(Path)), path_init(carryout);
    alu32_init(mips->alu, acuout, rdata1, rdata2, wdata, carryout);
    // // -------------
    // PCに実行開始アドレスを設定
    pc_set_value(mips->pc, INST_MEM_START); //0x04000000
    //メモリに命令を格納 例) add $t0, $t1, $t2 => 0x012a4020
    inst_memory_set_inst(mips->im, INST_MEM_START, inst);
    // //$t1(9番レジスタ)に0x100を代入
    registerfile_set_value(mips->regfile, 9, 0x300);
    //$t2(10番レジスタ)に0x300を代入
    registerfile_set_value(mips->regfile, 10, 0x100);
}

void mips_run(MIPS *mips) {
    pc_run(mips->pc);
    inst_memory_run(mips->im);
    split_bus_run(mips->sb);
    control_unit_run(mips->cu);
    printf("\naluop1 %d\n", path_get_signal(mips->cu->dups->out2));
    printf("\naluop1 %d\n", path_get_signal(mips->acu->agates->in1));    
    alu_control_unit_run(mips->acu);
    registerfile_run(mips->regfile);
    alu32_run(mips->alu);
    // ALUの計算結果を書き込むためにもう一度
    registerfile_run(mips->regfile);
    // $t0（8番レジスタ）の内容を表示する
    printf("$t0 = %x\n", registerfile_get_value(mips->regfile, 8));
}

void mips_test(int inst) {
    MIPS m;
    mips_init(&m, inst);
    mips_run(&m);
}