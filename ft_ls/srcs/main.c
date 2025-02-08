// allowed functions

// ◦ write
// ◦ opendir
// ◦ readdir
// ◦ closedir
// ◦ stat
// ◦ lstat
// ◦ getpwuid
// ◦ getgrgid
// ◦ listxattr
// ◦ getxattr
// ◦ time
// ◦ ctime
// ◦ readlink
// ◦ malloc
// ◦ free
// ◦ perror
// ◦ strerror
// ◦ exit

// errors:
// ls: cannot access '-': No such file or directory

// The stat(2) system call returns a struct stat that contains a st_mode member. This is the mode bits that ls -l displays.

// On my system, the man 2 stat page says:

//    The following flags are defined for the st_mode field:

//    S_IFMT     0170000   bitmask for the file type bitfields
//    S_IFSOCK   0140000   socket
//    S_IFLNK    0120000   symbolic link
//    S_IFREG    0100000   regular file
//    S_IFBLK    0060000   block device
//    S_IFDIR    0040000   directory
//    S_IFCHR    0020000   character device
//    S_IFIFO    0010000   FIFO
//    S_ISUID    0004000   set UID bit
//    S_ISGID    0002000   set-group-ID bit (see below)
//    S_ISVTX    0001000   sticky bit (see below)
//    S_IRWXU    00700     mask for file owner permissions
//    S_IRUSR    00400     owner has read permission
//    S_IWUSR    00200     owner has write permission
//    S_IXUSR    00100     owner has execute permission
//    S_IRWXG    00070     mask for group permissions
//    S_IRGRP    00040     group has read permission
//    S_IWGRP    00020     group has write permission
//    S_IXGRP    00010     group has execute permission
//    S_IRWXO    00007     mask for permissions for others (not in group)
//    S_IROTH    00004     others have read permission
//    S_IWOTH    00002     others have write permission
//    S_IXOTH    00001     others have execute permission

// FLAGS to follow:
// -l, -R, -a, -r and -t
// bouns: -u, -f, -g, -d

// find given documents -> eg: ls -la Documents/*.pdf -> only show pdfs

// todo: recursive lists of subdirectories, order and reverse order on output print
// sort by time insted of ascii

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

void print_file_data(t_entry *entry) {
	write(1, (S_ISDIR(entry->fileStat.st_mode))  ? "d" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IRUSR) ? "r" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IWUSR) ? "w" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IXUSR) ? "x" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IRGRP) ? "r" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IWGRP) ? "w" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IXGRP) ? "x" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IROTH) ? "r" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IWOTH) ? "w" : "-", 1);
	write(1, (entry->fileStat.st_mode & S_IXOTH) ? "x" : "-", 1);
	write(1, " ", 1);
}

void print_file_creation_time(t_entry *entry) {
	write(1, ctime(&entry->fileStat.st_ctime), 19);
	write(1, " ", 1);
}

void print_file_entry(t_entry *entry, t_command *command_structure) {
	if (find_in_string(command_structure->flags, 'l') == 1) {
		struct passwd *pwd = getpwuid(entry->fileStat.st_uid);
		struct group *grp = getgrgid(entry->fileStat.st_gid);
		print_file_data(entry);
		if (find_in_string(command_structure->flags, 'g') != 1)
			write(1, strcat(pwd->pw_name, " "), strlen(pwd->pw_name) + 1);
		write(1, strcat(grp->gr_name, " "), strlen(grp->gr_name) + 1);
		print_file_creation_time(entry);
	}
	write(1, entry->name, strlen(entry->name));
	write(1, find_in_string(command_structure->flags, 'l') == 1 ? "\n" : " ", 1);
}

int ft_strlen(char * string) {
	int index = 0;

	while (string[index] != '\0')
		index++;
	return (index);
}

void free_command_structure(t_command *command_structure) {
	int index = 0;

	if (command_structure->flags)
		free(command_structure->flags);
	if (command_structure->folder_list)
		while (command_structure->folder_list[index++])
			free(command_structure->folder_list[index]);
	free(command_structure);
}

void init_command_structure(t_command *command_structure) {
	command_structure->folder_count = 0;
	command_structure->multiple_folders = 0;
	command_structure->flags = malloc(sizeof(char*));
	command_structure->folder_list = malloc(sizeof(char**));
}

void sort_entries(t_entry *entries) {
	t_entry swap_space;
	int index = 0, found_flag = 0;

	while (entries[index].name) {
		while (entries[found_flag].name) {
			if (strcmp(entries[index].name, entries[found_flag].name) < 0) {
				// printf("[DEBUG] : swapping %s and %s\n", entries[index].name, entries[found_flag].name);
				swap_space = entries[index];
				entries[index] = entries[found_flag];
				entries[found_flag] = swap_space;
				// printf("[DEBUG] : swapped %s and %s\n", entries[index].name, entries[found_flag].name);
			}
			found_flag++;
		}
		found_flag = 0;
		index++;
	}
}

void sort_entries_by_time(t_entry *entries) {
	t_entry swap_space;
	int index = 0, found_flag = 0;

	while (entries[index].name) {
		while (entries[found_flag].name) {
			if (entries[index].fileStat.st_ctime > entries[found_flag].fileStat.st_ctime) {
				// printf("[DEBUG] : swapping %s and %s\n", entries[index].name, entries[found_flag].name);
				swap_space = entries[index];
				entries[index] = entries[found_flag];
				entries[found_flag] = swap_space;
				// printf("[DEBUG] : swapped %s and %s\n", entries[index].name, entries[found_flag].name);
			}
			found_flag++;
		}
		found_flag = 0;
		index++;
	}
}

void free_entries(t_entry *entries) {
	int index = 0;

	while (entries[index].name) {
		free(entries[index].name);
		index++;
	}
	free(entries);
}

int main(int argc, char *argv[]) {
	DIR			*directory;
	struct		dirent *pDirent;
	struct		stat fileStat;
	t_command	*command_structure = malloc(sizeof(t_command));
	t_entry *entries = NULL;

	if (!command_structure) {
		perror("[ERROR] : malloc failed");
		return 1;
	}

	char buffer[1024], *abspath;
	int folder_index = 0, entries_count = 0, i;

	init_command_structure(command_structure);
	parse_command(argc, argv, command_structure);

	while (command_structure->folder_list[folder_index]) {
		// printf("[DEBUG] : cycle in %s\n", command_structure->folder_list[folder_index]);
		if (command_structure->multiple_folders) {
			write(1, command_structure->folder_list[folder_index], strlen(command_structure->folder_list[folder_index]));
			write(1, ":\n", 2);
		}
		abspath = realpath(command_structure->folder_list[folder_index], buffer);
		if (!abspath) {
			printf("[DEBUG] : could not find absolute path.\n");
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
			if (pDirent->d_name[0] == '.' && find_in_string(command_structure->flags, 'a') != 1)
				continue;
			entries_count++;
		}
		closedir(directory);

		entries = malloc(sizeof(t_entry) * entries_count);
		if (!entries) {
			perror("[ERROR] : Malloc failed");
			free_command_structure(command_structure);
			return 1;
		}

		directory = opendir(abspath);
		i = 0;
		while ((pDirent = readdir(directory))) {
			if (pDirent->d_name[0] == '.' && find_in_string(command_structure->flags, 'a') != 1)
				continue;

			entries[i].name = strdup(pDirent->d_name);
			if (!entries[i].name) {
				perror("[ERROR] : Strdup failed");
				return 1;
			}
			stat(pDirent->d_name, &entries[i].fileStat);
			// printf("[DEBUG] : Saved to array: %s\n", entries[i].name);
			i++;
		}
		closedir(directory);

		if (find_in_string(command_structure->flags, 't') != 1)
			sort_entries(entries);
		else
			sort_entries_by_time(entries);

		if (find_in_string(command_structure->flags, 'r') != 1)
			for (i = 0; i < entries_count; i++)
				print_file_entry(&entries[i], command_structure);
		else
			for (i = entries_count - 1; i >= 0; i--)
				print_file_entry(&entries[i], command_structure);

		free_entries(entries);
		folder_index++;
		if (command_structure->folder_list[folder_index])
			write(1, "\n", 1);
	}
	if (find_in_string(command_structure->flags, 'l') != 1)
		write(1, "\n", 1);
	free_command_structure(command_structure);
	return 0;
}