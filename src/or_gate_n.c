#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void orgaten_init(ORGateN *ogaten, Path **ins, Path *out1, int num) {
    ogaten->num = num;
    ogaten->ins = ins;
    ogaten->out1 = out1;
}

void orgaten_run(ORGateN *ogaten) {
    int i;
    Signal val;
    val = false;
    for (i = 0; i < ogaten->num; i++) {
        // 条件判定を用いてよい
        if (path_get_signal(ogaten->ins[i])) {
            val = true;
            break;
        }
    }
    path_set_signal(ogaten->out1, val);
}