#ifndef MEMORY_H
#define MEMORY_H

struct mem_stats {
    long mem_total_kb;
    long mem_free_kb;
    long buffers_kb;
    long cached_kb;
};

int read_mem_stats(struct mem_stats *out);

#endif
