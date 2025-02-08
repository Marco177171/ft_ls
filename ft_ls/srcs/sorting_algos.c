#include "ft_ls.h"

void sort_entries(t_entry *entries) {
	int i, j, min_idx;
	t_entry temp;

	for (i = 0; entries[i].name; i++) {
		min_idx = i;
		for (j = i + 1; entries[j].name; j++) {
			if (strcmp(entries[j].name, entries[min_idx].name) < 0) {
				min_idx = j;
			}
		}
		if (min_idx != i) {
			// printf("[DEBUG] : swapping %s and %s\n", entries[i].name, entries[min_idx].name);
			temp = entries[i];
			entries[i] = entries[min_idx];
			entries[min_idx] = temp;
			// printf("[DEBUG] : swapped %s and %s\n", entries[i].name, entries[min_idx].name);
		}
	}
}

void sort_entries_by_time(t_entry *entries) {
	int i, j, min_idx;
	t_entry temp;

	for (i = 0; entries[i].name; i++) {
		min_idx = i;
		for (j = i + 1; entries[j].name; j++) {
			if (entries[j].fileStat.st_ctime < entries[min_idx].fileStat.st_ctime) {
				min_idx = j;
			}
		}
		if (min_idx != i) {
			// printf("[DEBUG] : swapping %s and %s\n", entries[i].name, entries[min_idx].name);
			temp = entries[i];
			entries[i] = entries[min_idx];
			entries[min_idx] = temp;
			// printf("[DEBUG] : swapped %s and %s\n", entries[i].name, entries[min_idx].name);
		}
	}
}