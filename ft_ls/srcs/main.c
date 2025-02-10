#include "ft_ls.h"

void free_entries(t_entry *entries, int entries_count) {
	int index = 0;

	while (index < entries_count) {
		free(entries[index].name);
		free(entries[index].fileStat);
		index++;
	}
	free(entries);
}

void execute_ls(t_command *command) {
	DIR *directory;
	struct dirent *pDirent;
	int index = 0;
	int entries_count = 0, recursive_index = 0;
	char buffer[1024];
	char *abspath;
	t_entry *entries = NULL;

	while (command->folder_list[index]) {
		
		if (index > 0)
			write(1, "\n", 1);
		
		if (command->multiple_folders || find_in_string(command->flags, 'R')) {
			write(1, command->folder_list[index], ft_strlen(command->folder_list[index]));
			write(1, ":\n", 2);
		}

		abspath = realpath(command->folder_list[index], buffer);
		if (!abspath) {
			index++;
			continue;
		}

		directory = opendir(abspath);
		if (!directory) {
			printf("[ERROR] : Could not open directory: %s\n", abspath);
			index++;
			continue;
		}

		// counting entries
		entries_count = 0;
		while ((pDirent = readdir(directory))) {
			if (pDirent->d_name[0] == '.' && find_in_string(command->flags, 'a') != 1)
				continue;
			entries_count++;
		}
		closedir(directory);

		// allocating space for entries list
		entries = malloc(sizeof(t_entry) * entries_count + 1);
		if (!entries) {
			perror("Memory allocation failed for entry list");
			exit(EXIT_FAILURE);
		}
		entries[entries_count].name = NULL;

		// save entries to list
		directory = opendir(abspath);
		if (!directory) {
			printf("[ERROR] : Could not open directory (SECOND): %s\n", abspath);
			index++;
			continue;
		}

		int entries_index = 0;
		while ((pDirent = readdir(directory))) {
			char buf[2048];
			memset(buf, 0, 2048);

			if (pDirent->d_name[0] == '.' && find_in_string(command->flags, 'a') != 1)
				continue;

			entries[entries_index].name = strdup(pDirent->d_name);
			if (!entries[entries_index].name) {
				perror("[ERROR] : Strdup failed");
				exit(EXIT_FAILURE);
			}
			entries[entries_index].fileStat = malloc(sizeof(struct stat));
			strcat(buf, abspath);
			strcat(buf, "/");
			strcat(buf, pDirent->d_name);
			lstat(buf, entries[entries_index].fileStat);

			entries_index++;
		}
		closedir(directory);

		sort(command, entries, entries_index);

		print_file_entry(entries, command, entries_index);

		if (find_in_string(command->flags, 'R') == 1) {
			recursive_index = 0;
			while (recursive_index < entries_index) {
				if (S_ISDIR(entries[recursive_index].fileStat->st_mode)) {
					if (strcmp(entries[recursive_index].name, ".") != 0 && strcmp(entries[recursive_index].name, "..") != 0) {  // Avoid loops
						char new_path[2048];
						memset(new_path, 0, 2048);
						snprintf(new_path, sizeof(new_path), "%s/%s", abspath, entries[recursive_index].name);
						t_command new_command = *command;
						new_command.folder_list = (char *[]) { new_path, NULL };
						write(1, "\n", 1);
						execute_ls(&new_command);
					}
				}
				recursive_index++;
			}
		}

		free_entries(entries, entries_index);

		index++;
	}
	if (find_in_string(command->flags, 'l') != 1)
		write(1, "\n", 1);
}

int main(int argc, char **argv) {
	t_command command_structure = {0};

	init_command(argc, argv, &command_structure);
	execute_ls(&command_structure);
	free_command(&command_structure);
	return 0;
}