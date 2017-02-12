#include <stdio.h>
#include <stdlib.h>
#include <mips.h>

void split_bus_init(SplitBus *sb, Bus *in1, Bus *out1, Bus *out2, Bus *out3, Bus *out4, Bus *out5) {
    sb->in1 = in1;
    sb->out1 = out1;
    sb->out2 = out2;
    sb->out3 = out3;
    sb->out4 = out4;
    sb->out5 = out5;
}

void split_bus_run(SplitBus *sb) {
    //in1の各PathのSignalを各outにsetする
    int i;

    // out1: 0-5
    for (i = 0; i <= 5; i++) {
        path_set_signal(bus_get_path(sb->out1, i), path_get_signal(bus_get_path(sb->in1, i)));
    }

    // out2: 11-15
    for (i = 11; i <= 15; i++) {
        path_set_signal(bus_get_path(sb->out2, i - 11), path_get_signal(bus_get_path(sb->in1, i)));
    }

    // out3: 16-20
    for (i = 16; i <= 20; i++) {
        path_set_signal(bus_get_path(sb->out3, i - 16), path_get_signal(bus_get_path(sb->in1, i)));
    }

    // out4: 21-25
    for (i = 21; i <= 25; i++) {
        path_set_signal(bus_get_path(sb->out4, i - 21), path_get_signal(bus_get_path(sb->in1, i)));
    }

    // out5: 26-31
    for (i = 26; i <= 31; i++) {
        path_set_signal(bus_get_path(sb->out5, i - 26), path_get_signal(bus_get_path(sb->in1, i)));
    }
}