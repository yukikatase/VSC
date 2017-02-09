#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void inst_memory_init(InstMemory *im, Bus *addr, Bus *inst) {
    im->mem = (int *)malloc(sizeof(int) * 1024); //1024 words
    //mem[i]: 1ワード(32bit)=>1命令文の情報を格納
    im->addr = addr;
    im->inst = inst;
}

void inst_memory_run(InstMemory *im) {
    // instバスにデータを送る
    // 命令アドレスが0x04000000から始まるので、
    // 0x04000000をmem[0]に対応させると効率がよい
    // memがワード単位なので4で割る
    int val, offset;
    val = bus_get_value(im->addr);
    offset = (val- INST_MEM_START) / 4;
    // mem[offset]の値をinstに設定
    bus_set_value(im->inst, im->mem[offset]);
}

void inst_memory_set_inst(InstMemory *im, int addr, int inst) {
    // 命令instをメモリに書き込む処理を書く
    // 0x04000000がmem[0]に対応
    im->mem[addr - INST_MEM_START] = inst;
}

void inst_memory_release() {}

void inst_memory_test() {
    Bus addr, inst;
    InstMemory im;
    bus_init(&addr, 32);
    bus_init(&inst, 32);
    inst_memory_init(&im, &addr, &inst);
    bus_set_value(&addr, 0x04000000);
    printf("%d\n", bus_get_value(&inst));
    //命令をセットする
    inst_memory_set_inst(&im, 0x04000000, 350);
    inst_memory_run(&im);
    printf("%d\n", bus_get_value(&inst));
    inst_memory_release(&im);
}