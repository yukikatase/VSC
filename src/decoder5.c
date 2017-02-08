#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void decoder5_init(Decoder5 *d, Bus *a, Bus *b) {
// 初期化
d->a = a;
d->b = b;
}

// 回路を用いずに作成してよい(if文等使用可)
void decoder5_run(Decoder5 *d) {
// バスbの内、aの値によって選ばれる配線の値だけを1に設定する（後の配線は0）
    bus_set_value(d->b, 0);
    path_set_signal(bus_get_path(d->b, bus_get_value(d->a)), 1);
}