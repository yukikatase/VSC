#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void dup_init(DUP *d, Path *in1, Path *out1, Path *out2) {
    d->in1 = in1;
    d->out1 = out1;
    d->out2 = out2;
}

void dup_run(DUP *d) {
    path_set_signal(d->out1, path_get_signal(d->in1));
    path_set_signal(d->out2, path_get_signal(d->in1));
}