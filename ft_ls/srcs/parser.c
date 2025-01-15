#include "ft_ls.h"

void parse_command(int argc, char *argv[], t_command *command_structure) {
	int index = 1;
	int flag_index = 0;
	int flag_string_index = 0;

	command_structure->multiple_folders = 0;
	command_structure->flags = malloc(sizeof(char*) * 256);
	command_structure->folder_count = 0;
	if (!command_structure->flags) {
		perror("[ERROR] : malloc failed");
		exit(EXIT_FAILURE);
	}
	command_structure->flags[0] = '\0'; // Initialize the string
	command_structure->folder_list = malloc(sizeof(char**) * 256);
	if (!command_structure->folder_list) {
		perror("[ERROR] : malloc on folder's list failed");
        free_command_structure(command_structure);
		exit(EXIT_FAILURE);
	}
	while (index < argc) {
		// printf("[DEBUG] : checking args: %s\n", argv[index]);
		if (argv[index][0] == '-') {
			// printf("[DEBUG] : new flag_list found\n");
			flag_index = 1;
			while (argv[index][flag_index]) {
				command_structure->flags[flag_string_index] = argv[index][flag_index];
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
			command_structure->folder_count++;
		}
		index++;
	}
    if (command_structure->folder_count == 0) {
        command_structure->folder_list[0] = strdup(".");
        if (!command_structure->folder_list[0]) {
            perror("[ERROR] : strdup failed");
            free(command_structure->flags);
            free(command_structure->folder_list);
            exit(EXIT_FAILURE);
        }
        command_structure->folder_count++;
    }
	command_structure->folder_count = 0;
	command_structure->flags[flag_string_index] = '\0';
	// printf("[DEBUG] : list of flags: %s\n", command_structure->flags);
}