#include "ft_ls.h"

void swap_entries(t_entry *a, t_entry *b) {
	t_entry temp = *a;
	*a = *b;
	*b = temp;
}

void sort_entries_by_time(t_entry *entries, int entries_amount) {
	int i = 0, j = 0;
	int smallest_index;

	while (i < entries_amount) {
		smallest_index = i;
		j = i + 1;
		while (j < entries_amount) {
			if (entries[i].fileStat->st_ctime > entries[j].fileStat->st_ctime)
				smallest_index = j;
			j++;
		}
		if (smallest_index > i)
			swap_entries(&entries[i], &entries[smallest_index]);
		i++;
	}
}

void sort_entries(t_entry *entries, int entries_amount) {
	int i = 0, j = 0;
	int smallest_index;

	while (i < entries_amount) {
		smallest_index = i;
		j = i + 1;
		while (j < entries_amount) {
			if (ft_strcmp(entries[j].name, entries[smallest_index].name) < 0)
				smallest_index = j;
			j++;
		}
		if (smallest_index > i)
			swap_entries(&entries[i], &entries[smallest_index]);
		i++;
	}
}

void sort(t_command *command, t_entry *entries, int entries_amount) {
	if (find_in_string(command->flags, 't') != 1)
		sort_entries(entries, entries_amount);
	else
		sort_entries_by_time(entries, entries_amount);
}