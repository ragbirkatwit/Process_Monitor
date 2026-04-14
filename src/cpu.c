#include <stdio.h>
#include "cpu.h"

int read_cpu_stats(struct cpu_stats *out) {
    FILE *fp = fopen("/proc/stat", "r");
    if (!fp) return -1;

    fscanf(fp, "cpu %llu %llu %llu %llu %llu %llu %llu",
           &out->user, &out->nice, &out->system, &out->idle, &out->iowait, &out->irq, &out->softirq);

    fclose(fp);
    return 0;
}
