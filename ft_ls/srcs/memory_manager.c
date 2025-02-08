#include "ft_ls.h"

void free_command_structure(t_command *command_structure) {
	int index = 0;

	if (command_structure->flags)
		free(command_structure->flags);
	if (command_structure->folder_list)
		while (command_structure->folder_list[index++])
			free(command_structure->folder_list[index]);
	free(command_structure);
}

void free_entries(t_entry *entries) {
	int index = 0;

	while (entries[index].name) {
		free(entries[index].name);
		index++;
	}
	free(entries);
}