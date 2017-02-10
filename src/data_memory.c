#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void data_memory_init(DataMemory *dm, Path *memWrite, Path *memRead, Bus *addr, Bus *wdata, Bus *rdata) {
    dm->mem = (int *)malloc(sizeof(int) * 1024); //1024 words
    dm->memWrite = memWrite;
    dm->memRead = memRead;
    dm->addr = addr;
    dm->wdata = wdata;
    dm->rdata = rdata;
}

void data_memory_run(DataMemory *dm) {
    // 本来ならmemReadとmemWriteが1かどうかはANDGateを
    // 使って判定するが、ここでは回路を用いないのでif文を使ってよい
    int val, offset;
    val = bus_get_value(dm->addr);
    offset = (val- DATA_MEM_START) / 4;
    // 読み出し処理（メモリの値をrdataに設定）
    if (path_get_signal(dm->memRead)) {
        bus_set_value(dm->rdata, dm->mem[offset]);
    }
    // 書き込み処理（wdataの値をメモリに設定）
    else if (path_get_signal(dm->memWrite)) {
        dm->mem[offset] = bus_get_value(dm->wdata);
    }
}

void data_memory_release(DataMemory *dm) {
    free(dm->mem);
}

void data_memory_test() {
    Path memWrite, memRead;
    Bus addr, wdata, rdata;
    DataMemory dm;

    bus_init(&addr, 32);
    bus_init(&wdata, 32);
    bus_init(&rdata, 32);

    path_init(&memWrite);
    path_init(&memRead);

    data_memory_init(&dm, &memWrite, &memRead, &addr, &wdata, &rdata);

    bus_set_value(&addr, 0x10000004);
    bus_set_value(&wdata, 100);

    path_set_signal(&memWrite, true);

    data_memory_run(&dm);

    bus_set_value(&addr, 0x10000008);
    bus_set_value(&wdata, 200);

    data_memory_run(&dm);

    printf("rdata : %d\n", bus_get_value(&rdata)); //0

    path_set_signal(&memWrite, false);
    path_set_signal(&memRead, true);

    data_memory_run(&dm);

    printf("rdata : %d\n", bus_get_value(&rdata)); //200

    bus_set_value(&addr, 0x10000004);

    data_memory_run(&dm);

    printf("rdata : %d\n", bus_get_value(&rdata)); //100

    data_memory_release(&dm);
}