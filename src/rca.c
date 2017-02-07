#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void rca_init(RCA *r, Bus *a, Bus *b, Path *carry_in, Bus *s, Path *carry_out) {
    Path *p;
    p = (Path *)malloc(sizeof(Path) * 31);
    r->f = (FA *)malloc(sizeof(FA) * 32);
    // Pathの初期化
    int i;
    for (i = 0; i < 31; i++) {
        path_init(p + i);
    }
    // 回路を作成（faの初期化を行う）
    for(i = 0; i < 32; i++){
        if(i == 0)
            fa_init(r->f + i, bus_get_path(a, i), bus_get_path(b, i), carry_in, bus_get_path(s, i), p);
        else if(i == 31)
            fa_init(r->f + i, bus_get_path(a, i), bus_get_path(b, i), p + i - 1, bus_get_path(s, i), carry_out);
        else
            fa_init(r->f + i, bus_get_path(a, i), bus_get_path(b, i), p + i - 1, bus_get_path(s, i), p + i);
    }
}

void rca_run(RCA *r) {
    int i;
    for (i = 0; i < 32; i++) {
        // 回路を実行
        fa_run(r->f + i);
    }
}

void rca_release(RCA *r) {
    int i;
    //各FAをreleaseする
    //確保(malloc)した領域を解放
    for(i = 0; i < 31; i++){
        free(fa_get_carry_out_path(r->f + i));
    }

    for(i = 0; i < 32; i++){
        fa_release(r->f + i);
    }
}

void rca_driver(int na, int nb) {
    RCA r;
    Bus a, b, s;
    Path carry_in, carry_out;
    // Pathの初期化
    path_init(&carry_in); path_init(&carry_out);
    // Busの初期化
    bus_init(&a, 32); bus_init(&b, 32); bus_init(&s, 32);
    // RCAにセット
    rca_init(&r, &a, &b, &carry_in, &s, &carry_out);
    bus_set_value(&a, na); bus_set_value(&b, nb);
    // RCAを実行
    rca_run(&r);
    printf("ADDER(%d, %d) => %d\n", na, nb, bus_get_value(&s));
    // 適宜release
    rca_release(&r);
}