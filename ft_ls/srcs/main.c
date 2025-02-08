#include "ft_ls.h"

int find_in_string(char *string, char character) {
	int index = 0;

	// printf("[DEBUG] : finding %c among flags. Flags: %s\n", character, string);
	while (string[index] != '\0') {
		// printf("[DEBUG] : searching at index %d: %c\n", index, string[index]);
		if (string[index] == character) {
			// printf("[DEBUG] : Found! Returning...\n");
			return (1);
		}
		index++;
	}
	// printf("[DEBUG] : Not found! Returning...\n");
	return (0);
}

int ft_strlen(char * string) {
	int index = 0;

	while (string[index] != '\0')
		index++;
	return (index);
}

int main(int argc, char *argv[]) {
	DIR			*directory;
	struct		dirent *pDirent;
	struct		stat fileStat;
	t_command	command_structure;
	t_entry *entries = NULL;

	char buffer[1024], *abspath;
	int folder_index = 0, entries_count = 0, i;

	parse_command(argc, argv, &command_structure);

	while (command_structure.folder_list[folder_index]) {
		// printf("[DEBUG] : cycle in %s\n", command_structure->folder_list[folder_index]);
		if (command_structure.multiple_folders) {
			write(1, command_structure.folder_list[folder_index], strlen(command_structure.folder_list[folder_index]));
			write(1, ":\n", 2);
		}
		abspath = realpath(command_structure.folder_list[folder_index], buffer);
		if (!abspath) {
			// printf("[DEBUG] : could not find absolute path.\n");
			folder_index++;
			continue;
		}

		directory = opendir(abspath);
		if (!directory) {
			printf("[ERROR] : Could not open directory: %s\n", abspath);
			folder_index++;
			continue;
		}

		entries_count = 0;
		while ((pDirent = readdir(directory))) {
			if (pDirent->d_name[0] == '.' && find_in_string(command_structure.flags, 'a') != 1)
				continue;
			entries_count++;
		}
		closedir(directory);

		directory = opendir(abspath);
		if (!directory) {
			printf("[ERROR] : Could not open directory (SECOND): %s\n", abspath);
			folder_index++;
			continue;
		}

		i = 0;
		while ((pDirent = readdir(directory))) {
			if (pDirent->d_name[0] == '.' && find_in_string(command_structure.flags, 'a') != 1)
				continue;

			entries[i].name = strdup(pDirent->d_name);
			if (!entries[i].name) {
				perror("[ERROR] : Strdup failed");
				return 1;
			}
			stat(pDirent->d_name, &entries[i].fileStat);
			printf("[DEBUG] : Found for array: %s\n", pDirent->d_name);
			printf("[DEBUG] : Saved to array: %s\n", entries[i].name);
			i++;
		}
		closedir(directory);

		if (find_in_string(command_structure.flags, 't') != 1)
			sort_entries(entries);
		else
			sort_entries_by_time(entries);

		if (find_in_string(command_structure.flags, 'r') != 1)
			for (i = 0; i < entries_count; i++)
				print_file_entry(&entries[i], &command_structure);
		else
			for (i = entries_count - 1; i >= 0; i--)
				print_file_entry(&entries[i], &command_structure);

		free_entries(entries);
		folder_index++;
		if (command_structure.folder_list[folder_index])
			write(1, "\n", 1);
	}
	if (find_in_string(command_structure.flags, 'l') != 1)
		write(1, "\n", 1);
	free_command_structure(&command_structure);
	return 0;
}