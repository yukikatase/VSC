#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void registerfile_init(RegisterFile *regfile, Path *regWrite, Bus *read1, Bus *read2, Bus *write1, Bus *wdata, Bus *rdata1, Bus *rdata2){
    regfile->agates = (ANDGate *)malloc(sizeof(ANDGate) * 31);
    regfile->regs = (Register *)malloc(sizeof(Register) * 32);
    regfile->dec = (Decoder5 *)malloc(sizeof(Decoder5));
    regfile->mux1 = (MUX32_Bus *)malloc(sizeof(MUX32_Bus));
    regfile->mux2 = (MUX32_Bus *)malloc(sizeof(MUX32_Bus));

    Bus *busd;
    busd = (Bus *)malloc(sizeof(Bus));
    bus_init(busd, 32);

    Path *patha;
    patha = (Path *)malloc(sizeof(Path) * 32);

    int i;
    for(i = 0; i < 32; i++){
        path_init(patha + i);
    }

    Bus *busr;
    busr = (Bus *)malloc(sizeof(Bus) * 32);
    for(i = 0; i < 32; i++){
        bus_init(busr + i, 32);
    }

    decoder5_init(regfile->dec, write1, busd);

    for(i = 0; i < 31; i++){
        andgate_init(regfile->agates + i, regWrite, bus_get_path(busd, i + 1), patha + i + 1);
    }
    
    for(i = 0; i < 32; i++){
        register_init(regfile->regs + i, patha + i, wdata, busr + i);
    }


    mux32_bus_init(regfile->mux1, read1, busr, rdata1);
    mux32_bus_init(regfile->mux2, read2, busr, rdata2);
}

void registerfile_run(RegisterFile *regfile){
// デコーダ、各レジスタとゲート、32ビットMUXをrunする
    decoder5_run(regfile->dec);
    int i;
    for(i = 0; i < 31; i++){
        andgate_run(regfile->agates + i);
    }
    for(i = 0; i < 32; i++){
        register_run(regfile->regs + i);
    }
    mux32_bus_run(regfile->mux1);
    mux32_bus_run(regfile->mux2);
}

void registerfile_release(RegisterFile *regfile){
    free(regfile->dec->b);
    free(regfile->dec);

    int i;
    ANDGate *a = regfile->agates;
    for(i = 0; i < 31; i++){
        free(a->out1);
        free(a);
        a++;
    }

    Register *r = regfile->regs;
    for(i = 0; i < 32; i++){
        free(r->rdata);
        free(r);
        r++;
    }

    free(regfile->mux1);
    free(regfile->mux2);
    
}
// 次の演習で用います
void registerfile_set_value(RegisterFile *regfile, int reg_num, int val){
    register_set_value(regfile->regs + reg_num, val);
}
int registerfile_get_value(RegisterFile *regfile, int reg_num){
    return register_get_value(regfile->regs + reg_num);
}

void registerfile_test(){
    Path reg_write;
    Bus read1, read2, write1, wdata, rdata1, rdata2;
    RegisterFile regfile; path_init(&reg_write);
    // 各Basを適切なPath数で初期化
    bus_init(&read1, 5); bus_init(&read2, 5);
    bus_init(&write1, 5);
    bus_init(&wdata, 32);
    bus_init(&rdata1, 32); bus_init(&rdata2, 32);
    registerfile_init(&regfile, &reg_write, &read1, &read2, &write1, &wdata, &rdata1, &rdata2);
    path_set_signal(&reg_write, true);
    bus_set_value(&read1, 1); // 読み出すレジスタ番号
    bus_set_value(&write1, 1); // 書き込むレジスタ番号
    bus_set_value(&wdata, 100); // 書き込む値
    registerfile_run(&regfile);
    printf("old data: %d\n", bus_get_value(&rdata1)); // 0
    // もう一度同じアクセス
    registerfile_run(&regfile);
    printf("new data: %d\n", bus_get_value(&rdata1)); // 100
    registerfile_release(&regfile);
}