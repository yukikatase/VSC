#include <stdio.h>
#include <mips.h>

void notgate_init(NOTGate *ngate, Path *in1, Path *out1) {
  ngate->in1 = in1;
  ngate->out1 = out1;
}

void notgate_run(NOTGate *ngate) {

  Signal s1, s2;
  s1 = path_get_signal(ngate->in1);
  s2 = !s1;
  path_set_signal(ngate->out1, s2);
}

void notgate_driver(Signal a) {
  
  Path in1, out1;
  NOTGate ngate;
  
  path_init(&in1);
  path_init(&out1);
  
  path_set_signal(&in1, a);
  
  notgate_init(&ngate, &in1, &out1);
  notgate_run(&ngate);
  
  printf("NOT(%d) => %d\n", a, path_get_signal(ngate.out1));
}
