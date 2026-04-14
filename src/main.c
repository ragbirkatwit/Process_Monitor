#include <stdio.h>
#include "cpu.h"
#include "memory.h"

int main() {
    struct cpu_stats cpu;
    struct mem_stats mem;

    read_cpu_stats(&cpu);
    read_mem_stats(&mem);

    printf("CPU Stats:\n");
    printf(" user=%llu nice=%llu system=%llu idle=%llu\n",
           cpu.user, cpu.nice, cpu.system, cpu.idle);

    printf("\nMemory Stats (kB):\n");
    printf(" Total=%ld Free=%ld Buffers=%ld Cached=%ld\n",
           mem.mem_total_kb, mem.mem_free_kb,
           mem.buffers_kb, mem.cached_kb);

    return 0;
}
