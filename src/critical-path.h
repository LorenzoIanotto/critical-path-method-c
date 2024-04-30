#ifndef CRITICAL_PATH_H
#define CRITICAL_PATH_H

#include <stdbool.h>
#include <stddef.h>

typedef char Activity;

typedef struct {
    Activity activity;
    size_t duration;
    struct {
        Activity *list;
        size_t len;
    } predecessors;
    bool earliest_available;
    bool latest_available;
    size_t earliest_start;
    size_t earliest_finish;
    size_t latest_start;
    size_t latest_finish;
    size_t slack_time;
} CPMTableEntry;

typedef struct {
    CPMTableEntry *list;
    size_t len;
} CPMTable;

/**
 * Returns -1 if an error occurs
 */
int solve_critical_path_method(CPMTable *table);

#endif // CRITICAL_PATH_H
