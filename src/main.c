#include "critical-path.h"
#include <stdio.h>
#include <stdlib.h>

void print_table(const CPMTable *table) {

    printf("\nAct\tLen\tES\tEF\tLS\tLF\tSL\n");
    for (size_t i = 0; i < table->len; ++i) {
        CPMTableEntry *current_entry = table->list + i;

        printf("%c\t%zu\t%zu\t%zu\t%zu\t%zu\t%zu\n", current_entry->activity,
               current_entry->duration, current_entry->earliest_start,
               current_entry->earliest_finish, current_entry->latest_start,
               current_entry->latest_finish, current_entry->slack_time);
    }
}

int main() {
    // Activity cde_predecessors[] = {'A'};
    // Activity f_predecessors[] = {'C'};
    // Activity g_predecessors[] = {'D'};
    // Activity h_predecessors[] = {'B', 'E'};
    // Activity i_predecessors[] = {'H'};
    // Activity j_predecessors[] = {'F', 'G', 'I'};
    //
    // CPMTableEntry entries[] = {
    //     {'A', 5, {NULL, 0}, false, false},
    //     {'B', 1, {NULL, 0}, false, false},
    //     {'C', 2, {cde_predecessors, 1}, false, false},
    //     {'D', 3, {cde_predecessors, 1}, false, false},
    //     {'E', 2, {cde_predecessors, 1}, false, false},
    //     {'F', 3, {f_predecessors, 1}, false, false},
    //     {'G', 4, {g_predecessors, 1}, false, false},
    //     {'H', 2, {h_predecessors, 2}, false, false},
    //     {'I', 1, {i_predecessors, 1}, false, false},
    //     {'J', 1, {j_predecessors, 3}, false, false},
    // };
    // CPMTable table = {.list = entries, .len = 10};

    puts("How many entries do you want to insert?");
    size_t len;
    int cnt = scanf("%zu", &len);

    if (cnt != 1) {
        puts("Error reading length");
        return 1;
    }

    if (len == 0) {
        return 0;
    }

    CPMTable table = {.list = malloc(len * sizeof(CPMTableEntry)), .len = len};
    for (size_t i = 0; i < len; ++i) {
        CPMTableEntry *new_entry = table.list + i;
        printf("Insert activity n. %zu\n", i + 1);

        printf("Insert name: ");
        int cnt;
        cnt = scanf(" %c", &new_entry->activity);

        if (cnt != 1) {
            puts("Failed parsing name");
            return 1;
        }

        printf("Insert duration: ");
        cnt = scanf("%zu", &new_entry->duration);

        if (cnt != 1) {
            puts("Failed parsing duration");
            return 1;
        }

        printf("Insert number of predecessors: ");
        cnt = scanf("%zu", &new_entry->predecessors.len);

        if (cnt != 1) {
            puts("Failed parsing number of predecessors");
            return 1;
        }

        new_entry->predecessors.list =
            malloc(new_entry->predecessors.len * sizeof(char));
        for (size_t j = 0; j < new_entry->predecessors.len; ++j) {
            printf("Predecessor name: ");
            cnt = scanf(" %c", new_entry->predecessors.list + j);

            if (cnt != 1) {
                puts("Failed parsing predecessor");
                return 1;
            }
        }
    }

    int err = solve_critical_path_method(&table);

    if (err) {
        puts("Failed to resolve table");
        return err;
    }

    print_table(&table);

    return 0;
}
