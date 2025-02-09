#include "ft_ls.h"

void init_command(int argc, char **argv, t_command *command_struct) {
	int index = 1, flag_index = 1, flag_string_length = 0;

	command_struct->folder_count = 0;
	command_struct->multiple_folders = 0;

	while (argv[index]) {
		if (argv[index][0] == '-' && argv[index][1]) {
			flag_index = 1;
			while (argv[index][flag_index]) {
				flag_string_length++;
				flag_index++;
			}
		}
		else
			command_struct->folder_count++;
		index++;
	}
	
	if (command_struct->folder_count > 1) {
		command_struct->multiple_folders = 1;
	}
	command_struct->flags = malloc((sizeof(char) * flag_string_length) + 1);
	command_struct->flags[flag_string_length] = '\0';
	if (!command_struct->flags) {
		perror("[ERROR] : Malloc failed on command's flag string");
		exit(EXIT_FAILURE);
	}

	command_struct->folder_list = malloc(sizeof(char*) * (command_struct->folder_count + 1));
	command_struct->folder_list[command_struct->folder_count] = NULL;
	if (!command_struct->folder_list) {
		perror("[ERROR] : Malloc failed on command's folders list");
		exit(EXIT_FAILURE);
	}
	if (command_struct->folder_count == 0) {
		command_struct->folder_list[0] = strdup(".");
	}
	
	index = 1;
	int flag_pos = 0, folder_pos = 0;
	while (index < argc) {
		if (argv[index][0] == '-' && argv[index][1]) {
			flag_index = 1;
			while (argv[index][flag_index]) {
				command_struct->flags[flag_pos++] = argv[index][flag_index];
				flag_index++;
			}
		} else {
			command_struct->folder_list[folder_pos] = strdup(argv[index]);
			if (!command_struct->folder_list[folder_pos]) {
				perror("[ERROR] : Malloc failed on folder name");
				exit(EXIT_FAILURE);
			}
			folder_pos++;
		}
		index++;
	}
	command_struct->flags[flag_pos] = '\0';
}

void free_command(t_command *command_structure) {
	int index = 0;

	while (command_structure->folder_list[index]) {
		free(command_structure->folder_list[index]);
		index++;
	}
	free(command_structure->folder_list);
	free(command_structure->flags);
}