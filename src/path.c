#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void path_set_signal(Path *p, Signal s) {
  p->s = s;
}

Signal path_get_signal(Path *p) {
  return p->s;
}

void path_init(Path *p) {
  p->s = false;
}