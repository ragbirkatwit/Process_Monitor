#ifndef PROC_PARSER_H
#define PROC_PARSER_H

struct process_info {
    int pid;
    char name[256];
    char state;

    unsigned long utime;
    unsigned long stime;

    long rss_kb;

    double cpu_percent;
};

int list_processes(struct process_info *list, int max);

#endif
