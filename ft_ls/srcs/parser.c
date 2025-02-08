#include "ft_ls.h"

void init_structure_content(int argc, char **argv, t_command *command_structure) {
	int index = 1, flag_index = 1, arg_count, flag_count = 0;
	
	command_structure->multiple_folders = 0;
	while (index < argc) {
		if (argv[index][0] == '-') {
			flag_index = 1;
			while (argv[index][flag_index]) {
				flag_index++;
				flag_count++;
			}
		}
		else
			command_structure->folder_count++;
		index++;
	}
	command_structure->flags = malloc(sizeof(char) * flag_count + 1);
	command_structure->flags[flag_count + 1] = '\0';
	if (!command_structure->flags) {
		perror("[ERROR] : Malloc failed for flags");
		exit(EXIT_FAILURE);
	}
	command_structure->folder_list = malloc(sizeof(char*) * command_structure->folder_count);
	command_structure->folder_list[command_structure->folder_count] = NULL;
	if (!command_structure->folder_list) {
		perror("[ERROR] : Malloc failed for folder list");
		exit(EXIT_FAILURE);
	}
}

// void list_this_folder_if_no_arg(t_command *command_structure) {
// 	if (command_structure->folder_count == 0) {
// 		command_structure->folder_list[0] = strdup(".");
// 		if (!command_structure->folder_list[0]) {
// 			perror("[ERROR] : strdup failed");
// 			free(command_structure->flags);
// 			free(command_structure->folder_list);
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// }

void fill_command_structure(int argc, char *argv[], t_command *command_structure) {
	int index = 1;
	int flag_index = 0;
	int flag_string_index = 0;
	command_structure->folder_count = 0;

	while (index < argc) {
		// printf("[DEBUG] : checking args: %s\n", argv[index]);
		if (argv[index][0] == '-') {
			// printf("[DEBUG] : new flag_list found\n");
			flag_index = 1;
			while (argv[index][flag_index]) {
				command_structure->flags[flag_string_index] = argv[index][flag_index];
				command_structure->flags[flag_string_index + 1] = '\0';
				flag_string_index++;
				flag_index++;
			}
		}
		else {
			if (command_structure->folder_count > 0)
				command_structure->multiple_folders = 1;
			command_structure->folder_list[command_structure->folder_count] = strdup(argv[index]);
			// printf("[DEBUG] : list of folders to write updated: %s\n", command_structure->folder_list[command_structure->folder_count]);
			if (!command_structure->folder_list[command_structure->folder_count]) {
				perror("[ERROR] : strdup failed");
				free_command_structure(command_structure);
				exit(EXIT_FAILURE);
			}
			command_structure->folder_list[command_structure->folder_count + 1] = NULL;
			command_structure->folder_count++;
		}
		index++;
	}
	// list_this_folder_if_no_arg(command_structure);
	command_structure->folder_list[index] = NULL;
	command_structure->folder_count = 0;
	command_structure->flags[flag_string_index] = '\0';
}

void parse_command(int argc, char *argv[], t_command *command_structure) {
	init_structure_content(argc, argv, command_structure);
	fill_command_structure(argc, argv, command_structure);
	// printf("[DEBUG] : list of flags: %s\n", command_structure->flags);
}