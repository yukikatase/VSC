#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void register_init(Register *reg, Path *wctl, Bus *wdata, Bus *rdata) {
    reg->val = 0;
    // 入力のPathとBusをreg内の各PathとBusに接続
    reg->wctl = wctl;
    reg->wdata = wdata;
    reg->rdata = rdata;
}

void register_run(Register *reg) {
// １．レジスタの値valをrdataに出力
// ２．wctlの信号が1ならwdataの値を書き込む
// （if文を使ってよい）
// 書き込む前の値を読み出せるように、読み出し、書き込みの
// 順番で行う必要がある（同時に読み書きする場合への対策）
    bus_set_value(reg->rdata, reg->val);
    if(path_get_signal(reg->wctl)){
        reg->val = bus_get_value(reg->wdata);
    }
}
// 次の演習で使います

void register_set_value(Register *reg, int v) {
reg->val = v;
}
int register_get_value(Register *reg) {
return reg->val;
}