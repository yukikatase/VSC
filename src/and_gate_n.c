#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void andgaten_init(ANDGateN *agaten, Path **ins, Path *out1, int num) {
    agaten->num = num;
    agaten->ins = ins;
    agaten->out1 = out1;
}

void andgaten_run(ANDGateN *agaten) {
    int i;
    Signal val;
    val = true;
    for (i = 0; i < agaten->num; i++) {
        // 条件判定を用いてよい
        if (!path_get_signal(agaten->ins[i])) {
            val = false;
            break;
        }
    }
    path_set_signal(agaten->out1, val);
}