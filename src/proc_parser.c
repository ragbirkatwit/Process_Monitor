#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "proc_parser.h"

int list_processes(struct process_info *list, int max) {
    DIR *d = opendir("/proc");
    if (!d) return 0;

    struct dirent *ent;
    int count = 0;

    while ((ent = readdir(d)) != NULL && count < max) {
        int pid = atoi(ent->d_name);
        if (pid <= 0) continue;

        struct process_info info;
        info.pid = pid;

        char path[64];
        sprintf(path, "/proc/%d/stat", pid);

        FILE *fp = fopen(path, "r");
        if (!fp) continue;

        int dummy;
        char comm[256], state;
        unsigned long utime, stime;

        fscanf(fp, "%d %s %c", &dummy, comm, &state);

        for (int i = 0; i < 11; i++) fscanf(fp, "%*s");

        fscanf(fp, "%lu %lu", &utime, &stime);
        fclose(fp);

        info.utime = utime;
        info.stime = stime;
        info.state = state;

        sprintf(path, "/proc/%d/status", pid);
        fp = fopen(path, "r");
        if (!fp) continue;

        char label[64];
        long value;
        char unit[16];

        info.rss_kb = 0;

        while (fscanf(fp, "%63s %ld %15s", label, &value, unit) == 3) {
            if (strcmp(label, "VmRSS:") == 0) {
                info.rss_kb = value;
                break;
            }
        }

        fclose(fp);

        list[count++] = info;
    }

    closedir(d);
    return count;
}
