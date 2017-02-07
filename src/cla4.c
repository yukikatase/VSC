#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void cla4_init(CLA4 *cla4, Bus *a, Bus *b, Path *carry_in, Bus *s, Path *carry_out, Path *G, Path *P) {
    Bus g, p, c;
    // 1. gとpを計算するためにPFAを用意
    cla4->pfa = (PFA *)malloc(sizeof(PFA) * 4);
    bus_init(&g, 4);
    bus_init(&p, 4);
    bus_init(&c, 5);

    pfa_init(cla4->pfa, bus_get_path(a, 0), bus_get_path(b, 0), carry_in, bus_get_path(&g, 0), bus_get_path(&p, 0), bus_get_path(s, 0));
    pfa_init(cla4->pfa + 1, bus_get_path(a, 1), bus_get_path(b, 1), bus_get_path(&c, 1), bus_get_path(&g, 1), bus_get_path(&p, 2), bus_get_path(s, 1));
    pfa_init(cla4->pfa + 2, bus_get_path(a, 2), bus_get_path(b, 2), bus_get_path(&c, 2), bus_get_path(&g, 2), bus_get_path(&p, 3), bus_get_path(s, 2));
    pfa_init(cla4->pfa + 3, bus_get_path(a, 3), bus_get_path(b, 3), bus_get_path(&c, 3), bus_get_path(&g, 3), bus_get_path(&p, 4), bus_get_path(s, 3));

    // 2. cを計算するためにCLUを用意
    cla4->clu = (CLU *)malloc(sizeof(CLU));
    path_set_signal(bus_get_path(&c, 0), path_get_signal(carry_in));
    clu_init(cla4->clu, &g, &p, &c, carry_out);
    // 3. sを計算するためにXORCircuitを用意
    // PFAの出力をうまく用いる
    cla4->xorc = (XORCircuit *)malloc(sizeof(XORCircuit) *  6);
    Path *path;
    path = (Path *)malloc(sizeof(Path) * 3);
    path_init(path); path_init(path + 1); path_init(path + 2);

    xorcircuit_init(cla4->xorc, bus_get_path(a, 1), bus_get_path(b, 1), path);
    xorcircuit_init(cla4->xorc + 1, bus_get_path(&c, 1), path, s->p + 1);
    xorcircuit_init(cla4->xorc + 2, bus_get_path(a, 2), bus_get_path(b, 2), path + 1);
    xorcircuit_init(cla4->xorc + 3, bus_get_path(&c, 2), path + 1, s->p + 2);
    xorcircuit_init(cla4->xorc + 4, bus_get_path(a, 3), bus_get_path(b, 3), path + 2);
    xorcircuit_init(cla4->xorc + 5, bus_get_path(&c, 3), path + 2, s->p + 3);

    // 4. GとPを計算するためにANDGateNとORGateNを用意
    // 次の次のページを参照して各GとPの計算を行うための回路を作成
    cla4->agaten = (ANDGateN *)malloc(sizeof(ANDGateN) * 4);
    cla4->ogaten = (ORGateN *)malloc(sizeof(ORGateN));

    Path *path2;
    path2 = (Path *)malloc(sizeof(Path) * 3);
    path_init(path2); path_init(path2 + 1); path_init(path2 + 2);

    Path **ps1, **ps2, **ps3, **ps4, **ps5;

    ps1 = (Path **)malloc(sizeof(Path*) * 2);
    ps1[0] = bus_get_path(&p, 3);
    ps1[1] = bus_get_path(&g, 2);

    ps2 = (Path **)malloc(sizeof(Path*) * 3);
    ps2[0] = bus_get_path(&p, 3);
    ps2[1] = bus_get_path(&p, 2);
    ps2[2] = bus_get_path(&g, 1);

    ps3 = (Path **)malloc(sizeof(Path*) * 4);
    ps3[0] = bus_get_path(&p, 3);
    ps3[1] = bus_get_path(&p, 2);
    ps3[2] = bus_get_path(&p, 1);
    ps3[3] = bus_get_path(&g, 0);

    ps4 = (Path **)malloc(sizeof(Path*) * 4);
    ps4[0] = bus_get_path(&g, 3);
    ps4[1] = path2;
    ps4[2] = path2 + 1;
    ps4[3] = path2 + 2;


    ps5 = (Path **)malloc(sizeof(Path*) * 4);
    ps5[0] = bus_get_path(&p, 0);
    ps5[1] = bus_get_path(&p, 1);
    ps5[2] = bus_get_path(&p, 2);
    ps5[3] = bus_get_path(&p, 3);

    andgaten_init(cla4->agaten, ps1, path2, 2);
    andgaten_init(cla4->agaten + 1, ps2, path2 + 1, 3);
    andgaten_init(cla4->agaten + 2, ps3, path2 + 2, 4);

    orgaten_init(cla4->ogaten, ps4, G, 4);

    andgaten_init(cla4->agaten + 3, ps5, P, 4);
    // Path**を用意して、ANDGateNやORGateNに引数として渡す
}

void cla4_run(CLA4 *cla4) {
    // 各回路を実行
    pfa_run(cla4->pfa);
    printf("PFA g:%d, p:%d, s:%d\n", path_get_signal(cla4->pfa->agate1->out1), path_get_signal(cla4->pfa->ogate1->out1), path_get_signal(cla4->pfa->f1->xorc2->ogate1->out1));
    pfa_run(cla4->pfa + 1);
    pfa_run(cla4->pfa + 2);
    pfa_run(cla4->pfa + 3);
    clu_run(cla4->clu);
    XORCircuit *a = cla4->xorc + 3;
    printf("CLU g:%d, p:%d, c1:%d\n", path_get_signal(cla4->clu->ogates->in2), path_get_signal(cla4->clu->agates->in1), path_get_signal(a->ngate1->in1));
    xorcircuit_run(cla4->xorc);
    xorcircuit_run(cla4->xorc + 1);
    xorcircuit_run(cla4->xorc + 2);
    xorcircuit_run(cla4->xorc + 3);
    xorcircuit_run(cla4->xorc + 4);
    xorcircuit_run(cla4->xorc + 5);
    andgaten_run(cla4->agaten);
    andgaten_run(cla4->agaten + 1);
    andgaten_run(cla4->agaten + 2);
    andgaten_run(cla4->agaten + 3);
    orgaten_run(cla4->ogaten);
}

void cla4_release(CLA4 *cla4) {
    // 確保した領域を解放
    pfa_release(cla4->pfa);
    clu_release(cla4->clu);
    XORCircuit *a = cla4->xorc;
    free(a->ogate1->out1);
    xorcircuit_release(a); a++;
    free(a->ogate1->out1);
    xorcircuit_release(a); a++;
    free(a->ogate1->out1);
    xorcircuit_release(a); a++;
    free(cla4->pfa);
    free(cla4->clu);
    free(cla4->xorc);
    ANDGateN *b = cla4->agaten;
    free(b->out1); free(b->ins); b++;
    free(b->out1); free(b->ins); b++;
    free(b->out1); free(b->ins); b++;

    free(cla4->agaten);
    free(cla4->ogaten);
}

void cla4_driver(int na, int nb) {
    CLA4 c;
    // PathやBusを用意
    Bus a, b, s;
    bus_init(&a, 4); bus_init(&b, 4); bus_init(&s, 4);

    Path carry_in, carry_out, G, P;
    path_init(&carry_in); path_init(&carry_out);
    path_init(&G); path_init(&P);
    path_set_signal(&carry_in, 1);
    // CLA4の初期化
    cla4_init(&c, &a, &b, &carry_in, &s, &carry_out, &G, &P);
    // Busに値をセット
    bus_set_value(&a, na);
    bus_set_value(&b, nb);
    // CLA4の実行
    cla4_run(&c);
    // 結果の出力
    printf("ADDER(%d, %d) carry_in = 1 => %d\n", na, nb, bus_get_value(&s));
    // 適宜release
    cla4_release(&c);
}