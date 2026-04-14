#include <stdio.h>
#include <string.h>
#include "memory.h"

int read_mem_stats(struct mem_stats *out) {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) return -1;

    char label[64];
    long value;
    char unit[16];

    while (fscanf(fp, "%63s %ld %15s", label, &value, unit) == 3) {
        if (strcmp(label, "MemTotal:") == 0)
            out->mem_total_kb = value;
        else if (strcmp(label, "MemFree:") == 0)
            out->mem_free_kb = value;
        else if (strcmp(label, "Buffers:") == 0)
            out->buffers_kb = value;
        else if (strcmp(label, "Cached:") == 0)
            out->cached_kb = value;
    }

    fclose(fp);
    return 0;
}
