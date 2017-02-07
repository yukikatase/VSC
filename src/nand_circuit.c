#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void nandcircuit_init(NANDCircuit *nandc, Path *in1, Path *in2, Path *out1) {

  Path *inner;
  inner = (Path *)malloc(sizeof(Path));
  path_init(inner);
  nandc->agate1 = (ANDGate *)malloc(sizeof(ANDGate));
  nandc->ngate1 = (NOTGate *)malloc(sizeof(NOTGate));
  andgate_init(nandc->agate1, in1, in2, inner);
  notgate_init(nandc->ngate1, inner, out1);
}

void nandcircuit_run(NANDCircuit *nandc) {
  andgate_run(nandc->agate1);
  notgate_run(nandc->ngate1);
}

void nandcircuit_release(NANDCircuit *nandc) {
  free(nandc->agate1->out1);
  free(nandc->agate1);
  free(nandc->ngate1);
}

Path* nandcircuit_get_out_path(NANDCircuit *nandc) {
  return nandc->ngate1->out1;
}

void nandcircuit_driver(Signal a, Signal b) {
  
  Path in1, in2, out1;
  NANDCircuit nandc;

  path_init(&in1);
  path_init(&in2);
  path_init(&out1);
  
  path_set_signal(&in1, a);
  path_set_signal(&in2, b);
  
  nandcircuit_init(&nandc, &in1, &in2, &out1);
  nandcircuit_run(&nandc);
  
  printf("NAND(%d, %d) => %d\n", a, b, path_get_signal(nandcircuit_get_out_path(&nandc)));

  nandcircuit_release(&nandc);

}
