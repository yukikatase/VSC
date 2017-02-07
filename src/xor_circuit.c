#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void xorcircuit_init(XORCircuit *nandc, Path *in1, Path *in2, Path *out1) {

  Path *inner1, *inner2, *inner3, *inner4;
  inner1 = (Path *)malloc(sizeof(Path));
  inner2 = (Path *)malloc(sizeof(Path));
  inner3 = (Path *)malloc(sizeof(Path));
  inner4 = (Path *)malloc(sizeof(Path));

  path_init(inner1);
  path_init(inner2);
  path_init(inner3);
  path_init(inner4);

  nandc->ngate1 = (NOTGate *)malloc(sizeof(NOTGate));
  nandc->ngate2 = (NOTGate *)malloc(sizeof(NOTGate));
  nandc->agate1 = (ANDGate *)malloc(sizeof(ANDGate));
  nandc->agate2 = (ANDGate *)malloc(sizeof(ANDGate));
  nandc->ogate1 = (ORGate *)malloc(sizeof(ORGate));

  notgate_init(nandc->ngate1, in1, inner1);
  notgate_init(nandc->ngate2, in2, inner2);
  andgate_init(nandc->agate1, inner1, in2, inner3);
  andgate_init(nandc->agate2, in1, inner2, inner4);
  orgate_init(nandc->ogate1, inner3, inner4, out1);
}

void xorcircuit_run(XORCircuit *nandc) {
  notgate_run(nandc->ngate1);
  notgate_run(nandc->ngate2);
  andgate_run(nandc->agate1);
  andgate_run(nandc->agate2);
  orgate_run(nandc->ogate1);
}

void xorcircuit_release(XORCircuit *nandc) {
  free(nandc->ngate1->out1);
  free(nandc->ngate2->out1);
  free(nandc->agate1->out1);
  free(nandc->agate2->out1);

  free(nandc->ngate1);
  free(nandc->ngate2);
  free(nandc->agate1);
  free(nandc->agate2);
  free(nandc->ogate1);
}

Path* xorcircuit_get_out_path(XORCircuit *nandc) {
  return nandc->ogate1->out1;
}

void xorcircuit_driver(Signal a, Signal b) {

  Path in1, in2, out1;
  XORCircuit nandc;

  path_init(&in1);
  path_init(&in2);
  path_init(&out1);

  path_set_signal(&in1, a);
  path_set_signal(&in2, b);

  xorcircuit_init(&nandc, &in1, &in2, &out1);
  xorcircuit_run(&nandc);

  printf("XOR(%d, %d) => %d\n", a, b, path_get_signal(xorcircuit_get_out_path(&nandc)));

  xorcircuit_release(&nandc);

}
