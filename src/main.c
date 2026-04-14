#include <stdio.h>
#include <unistd.h>
#include "cpu.h"
#include "memory.h"
#include "proc_parser.h"

int main() {
    struct cpu_stats cpu, last_cpu = {0};
    struct process_info list[2048];

    // Tracks per-process CPU deltas
    static unsigned long last_proc_time[65536] = {0};

    while (1) {
        // 1. Read all processes
        int count = list_processes(list, 2048);

        // 2. Read system CPU stats
        read_cpu_stats(&cpu);

        unsigned long long total =
            cpu.user + cpu.nice + cpu.system + cpu.idle +
            cpu.iowait + cpu.irq + cpu.softirq;

        unsigned long long last_total =
            last_cpu.user + last_cpu.nice + last_cpu.system + last_cpu.idle +
            last_cpu.iowait + last_cpu.irq + last_cpu.softirq;

        unsigned long long total_delta = total - last_total;

        // Save snapshot for next iteration
        last_cpu = cpu;

        // 3. Compute CPU% for each process
        for (int i = 0; i < count; i++) {
            unsigned long proc_time = list[i].utime + list[i].stime;
            unsigned long delta = proc_time - last_proc_time[list[i].pid];
            last_proc_time[list[i].pid] = proc_time;

            if (total_delta > 0)
                list[i].cpu_percent = (100.0 * delta) / total_delta;
            else
                list[i].cpu_percent = 0.0;
        }

        // 4. Sort by CPU%
        for (int i = 0; i < count - 1; i++) {
            for (int j = i + 1; j < count; j++) {
                if (list[j].cpu_percent > list[i].cpu_percent) {
                    struct process_info tmp = list[i];
                    list[i] = list[j];
                    list[j] = tmp;
                }
            }
        }

        // 5. Clear screen
        printf("\033[2J\033[H");

        printf("PID     CPU%%     RSS(KB)     STATE\n");
        printf("--------------------------------------\n");

        // 6. Print top 20 processes
        for (int i = 0; i < 20 && i < count; i++) {
            printf("%5d   %6.2f   %8ld      %c\n",
                   list[i].pid,
                   list[i].cpu_percent,
                   list[i].rss_kb,
                   list[i].state);
        }

        usleep(500000); // refresh every 0.5 seconds
    }

    return 0;
}
