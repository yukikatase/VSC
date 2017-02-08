#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void registerfile_init(RegisterFile *regfile, Path *regWrite, Bus *read1, Bus *read2, Bus *write1, Bus *wdata, Bus *rdata1, Bus *rdata2) { // 出力
    // レジスタを32個用意
    regfile->agates = (ANDGate *)malloc(sizeof(ANDGate) * 31);
    regfile->regs = (Register *)malloc(sizeof(Register) * 32);
    regfile->dec = (Decoder5 *)malloc(sizeof(Decoder));
    regfile->mux1 = (MUX32_Bus *)malloc(sizeof(MUX32_Bus));
    regfile->mux2 = (MUX32_Bus *)malloc(sizeof(MUX32_Bus));    
    // 各種配線と初期化
    register_init(regfile->reg, regWrite, wdata, )
    regfile->mux1->ctls = read1;
    regfile->mux2->ctls = read2;
    regfile->dec->a = write1;

}

void registerfile_run(RegisterFile *regfile) {
// デコーダ、各レジスタとゲート、32ビットMUXをrunする
}
void registerfile_release(RegisterFile *regfile) {

}
// 次の演習で用います
void registerfile_set_value(RegisterFile *regfile, int reg_num, int val) {
    register_set_value(regfile->regs + reg_num, val);
}
int registerfile_get_value(RegisterFile *regfile, int reg_num) {
    return register_get_value(regfile->regs + reg_num);
}

void registerfile_test() {
    Path reg_write; Bus read1, read2, write1, wdata, rdata1, rdata2;
    RegisterFile regfile; path_init(&reg_write);
    // 各Basを適切なPath数で初期化
    bus_init(&read1, 5); bus_init(&read2, 5);
    bus_init(&write1, 5);
    bus_init(&wdata, 32);
    bus_init(&rdata1, 32); bus_init(&rdata2, 32);
    registerfile_init(&regfile, &reg_write, &read1, &read2,
    &write1, &wdata, &rdata1, &rdata2);
    path_set_signal(&reg_write, true);
    bus_set_value(&read1, 1); // 読み出すレジスタ番号
    bus_set_value(&write1, 1); // 書き込むレジスタ番号
    bus_set_value(&wdata, 100); // 書き込む値
    registerfile_run(&regfile);
    printf(“old data: %d¥n”, bus_get_value(&rdata1)); // 0
    // もう一度同じアクセス
    registerfile_run(&regfile);
    printf("new data: %d¥n", bus_get_value(&rdata1)); // 100
}