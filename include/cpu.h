#ifndef CPU_H
#define CPU_H

struct cpu_stats {
    unsigned long long user;
    unsigned long long nice;
    unsigned long long system;
    unsigned long long idle;
    unsigned long long iowait;
    unsigned long long irq;
    unsigned long long softirq;
};

int read_cpu_stats(struct cpu_stats *out);

#endif
