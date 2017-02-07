#include <stdio.h>
#include <mips.h>

void andgate_init(ANDGate *agate, Path *in1, Path *in2, Path *out1) {
  agate->in1 = in1;
  agate->in2 = in2;
  agate->out1 = out1;
}

void andgate_run(ANDGate *agate) {

  Signal s1, s2, s3;
  s1 = path_get_signal(agate->in1);
  s2 = path_get_signal(agate->in2);
  
  s3 = s1 && s2;
  path_set_signal(agate->out1, s3);
  
}

void andgate_driver(Signal a, Signal b) {
  
  Path in1, in2, out1;
  ANDGate agate;
  
  path_init(&in1);
  path_init(&in2);
  path_init(&out1);
  
  path_set_signal(&in1, a);
  path_set_signal(&in2, b);
  
  andgate_init(&agate, &in1, &in2, &out1);
  andgate_run(&agate);
  
  printf("AND(%d, %d) => %d\n", a, b, path_get_signal(agate.out1));
  
}
