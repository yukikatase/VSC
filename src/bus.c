#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void bus_init(Bus *b, int n) {
    int i;
    b->num = n;
    b->p = (Path *)malloc(sizeof(Path) * n);
    for (i = 0; i < n; i++) {
        path_init(b->p + i);
    }
}

void bus_set_value(Bus *b, int val) {
    int i;
    for (i = 0; i < b->num; i++) {
        if ((val & 0x1) != 0){
            path_set_signal(b->p + i, true);
        }else{
            path_set_signal(b->p + i, false);
        }
        val >>= 1; // 1ビット右シフト
    }
}

int bus_get_value(Bus *b) {
    int i, val = 0;
    for (i = b->num - 1; i >= 0; i--) {
        val <<= 1; // 1ビット左シフト
        if (path_get_signal(b->p + i))
            val += 1; // 信号が1ならビットを立てる
    }
    return val;
}

Path* bus_get_path(Bus *b, int i) {
    return b->p + i;
}

void bus_release(Bus *b) {
    free(b->p);
}