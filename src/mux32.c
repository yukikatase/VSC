#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void mux32_bus_init(MUX32_Bus *mb, Bus *ctls, Bus *ins, Bus *out1) {
mb->ctls = ctls;
mb->ins = ins;
mb->out1 = out1;
}
// 回路を用いずに作成してよい(if文等使用可)
void mux32_bus_run(MUX32_Bus *mb) {
// ctlsの値によって選択される入力バスの値を出力バスに設定する
    bus_set_value(mb->out1, bus_get_value(mb->ins + bus_get_value(mb->ctls)));
}