#include "critical-path.h"

CPMTableEntry *get_by_activity(const CPMTable *table, Activity activity) {
    for (size_t i = 0; i < table->len; ++i) {
        CPMTableEntry *current_entry = table->list + i;

        if (current_entry->activity == activity) {
            return current_entry;
        }
    }

    return NULL;
}

/**
 * Returns NULL if all the nodes' earliest_start and earliest_finish are filled
 */
CPMTableEntry *
get_first_available_activity_for_earliest(const CPMTable *table) {
    for (size_t entries_index = 0; entries_index < table->len;
         ++entries_index) {
        CPMTableEntry *current_entry = table->list + entries_index;

        if (current_entry->earliest_available) {
            continue;
        }

        bool all_predecessor_have_available_earliest = true;
        for (size_t predecessors_index = 0;
             predecessors_index < current_entry->predecessors.len;
             ++predecessors_index) {
            CPMTableEntry *predecessor_entry = get_by_activity(
                table, current_entry->predecessors.list[predecessors_index]);

            if (!predecessor_entry->earliest_available) {
                all_predecessor_have_available_earliest = false;
                break;
            }
        }

        if (all_predecessor_have_available_earliest) {
            return current_entry;
        }
    }

    return NULL;
}

CPMTableEntry *get_entry_with_maximum_ef_without_latest(const CPMTable *table) {
    CPMTableEntry *max_ef_entry = NULL;

    for (size_t i = 0; i < table->len; ++i) {
        CPMTableEntry *current_entry = table->list + i;

        if (current_entry->latest_available) {
            continue;
        }

        if (max_ef_entry == NULL) {
            max_ef_entry = current_entry;
            continue;
        }

        if (current_entry->earliest_finish > max_ef_entry->earliest_finish) {
            max_ef_entry = current_entry;
        }
    }

    return max_ef_entry;
}

void fill_earliest_in_first_activities(CPMTable *table) {
    for (size_t i = 0; i < table->len; ++i) {
        CPMTableEntry *current_entry = table->list + i;

        if (current_entry->predecessors.len == 0) {
            current_entry->earliest_start = 0;
            current_entry->earliest_finish = current_entry->duration;
            current_entry->earliest_available = true;
        }
    }
}

bool is_a_predecessor_of(const CPMTable *table, const CPMTableEntry *entry,
                         const CPMTableEntry *supposed_predecessor_entry) {
    for (size_t i = 0; i < entry->predecessors.len; ++i) {
        Activity current_predecessor = entry->predecessors.list[i];

        if (supposed_predecessor_entry->activity == current_predecessor) {
            return true;
        }
    }

    return false;
}

void fill_path(CPMTable *table, CPMTableEntry *head, const size_t slack_time) {
    if (!head->latest_available) {
        head->slack_time = slack_time;
        head->latest_start = head->earliest_start + slack_time;
        head->latest_finish = head->earliest_finish + slack_time;
        head->latest_available = true;
    }

    if (head->predecessors.len == 0) {
        return;
    }

    for (size_t i = 0; i < table->len; ++i) {
        CPMTableEntry *current_entry = table->list + i;

        if (current_entry->earliest_finish == head->earliest_start &&
            is_a_predecessor_of(table, head, current_entry)) {
            fill_path(table, current_entry, slack_time);
        }
    }
}

int solve_critical_path_method(CPMTable *table) {
    fill_earliest_in_first_activities(table);

    while (1 /** Not solved*/) {
        CPMTableEntry *current_entry =
            get_first_available_activity_for_earliest(table);

        if (current_entry == NULL) {
            break;
        }

        CPMTableEntry *max_ef_pred_entry = NULL;

        for (size_t i = 0; i < current_entry->predecessors.len; ++i) {

            CPMTableEntry *predecessor_entry =
                get_by_activity(table, current_entry->predecessors.list[i]);

            if (max_ef_pred_entry == NULL) {
                max_ef_pred_entry = predecessor_entry;
            }

            if (predecessor_entry->earliest_finish >
                max_ef_pred_entry->earliest_finish) {
                max_ef_pred_entry = predecessor_entry;
            }
        }

        current_entry->earliest_start = max_ef_pred_entry->earliest_finish;
        current_entry->earliest_finish =
            max_ef_pred_entry->earliest_finish + current_entry->duration;
        current_entry->earliest_available = true;
    }

    CPMTableEntry *longest = get_entry_with_maximum_ef_without_latest(table);
    CPMTableEntry *current_entry = longest;
    while (current_entry != NULL) {
        size_t slack_time =
            longest->earliest_finish - current_entry->earliest_finish;
        fill_path(table, current_entry, slack_time);

        current_entry = get_entry_with_maximum_ef_without_latest(table);
    }

    return 0;
}
