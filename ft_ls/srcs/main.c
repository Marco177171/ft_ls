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

void bubble_sort_output(char **entry_names) {
	char	*temp;
	int		entries_index = 0;
	int		name_index = 0;

	temp = malloc(sizeof(entry_names));
	while (entry_names[entries_index + 1]) {
		// printf("[DEBUG] : %s is a directory!\n", entry_names[entries_index]);
		while (entry_names[entries_index][name_index] && entry_names[entries_index]) {
			if (entry_names[entries_index + 1][name_index] < entry_names[entries_index][name_index]) {	
				// printf("[DEBUG] : switching %s with %s on index %d\n", entry_names[entries_index], entry_names[entries_index + 1], name_index);
				temp = strdup(entry_names[entries_index]);
				entry_names[entries_index] = strdup(entry_names[entries_index + 1]);
				entry_names[entries_index + 1] = strdup(temp);
				free(temp);
			}
			name_index++;
		}
		temp = malloc(sizeof(char*));
		free(temp);
		entries_index++;
	}
}

void print_file_data(struct dirent *pDirent) {
	struct stat fileStat;

	stat(pDirent->d_name, &fileStat);
	// if (S_ISDIR(fileStat.st_mode))
	// 	printf("[DEBUG] : %s is a directory!\n", pDirent->d_name);
	printf((S_ISDIR(fileStat.st_mode))  ? "d" : "-");
	printf((fileStat.st_mode & S_IRUSR) ? "r" : "-");
	printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
	printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");
	printf((fileStat.st_mode & S_IRGRP) ? "r" : "-");
	printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
	printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");
	printf((fileStat.st_mode & S_IROTH) ? "r" : "-");
	printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
	printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");
	printf(" ");
}

void print_file_creation_time(char *path) {
    struct stat attr;

    stat(path, &attr);
    printf("%s", ctime(&attr.st_mtime));
}

void print_file_entry(struct dirent *pDirent, t_command *command_structure) {
	if (find_in_string(command_structure->flags, 'l') == 1) {
		print_file_data(pDirent);
		print_file_creation_time(pDirent->d_name);
	}
	printf("%s", pDirent->d_name);
	// if (find_in_string(command_structure->flags, 'l') == 1)
	// 	printf("\n");
	
	printf(find_in_string(command_structure->flags, 'l') == 1 ? "\n" : "  ");
	// printf("It's d_ino is %ld\t", pDirent->d_ino);
	// printf("It's d_off is %ld\t", pDirent->d_off);
	// printf("It's d_type is %d\n", pDirent->d_type);
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
}

void init_command_structure(t_command *command_structure) {
	command_structure->folder_count = 0;
	command_structure->multiple_folders = 0;
	command_structure->flags = malloc(sizeof(char*));
	command_structure->folder_list = malloc(sizeof(char**));
}

int main(int argc, char *argv[]) {
	DIR				*directory;
	struct dirent	*pDirent;
	struct stat		fileStat;
	t_command		*command_structure;
	char			buffer[1024];
	char			*abspath;
	int				folder_index = 0;
	int				list_index = 0;
	// time_t			current_time;

	init_command_structure(command_structure);
	parse_command(argc, argv, command_structure);
	while (command_structure->folder_list[folder_index]) {
		// printf("[DEBUG] : folder at %d : %s\n", folder_index, command_structure->folder_list[folder_index]);
		folder_index++;
	}
	folder_index = 0;
	while (command_structure->folder_list[folder_index]) {
		// printf("[DEBUG] : cycle in %s\n", command_structure->folder_list[folder_index]);
		abspath = realpath(command_structure->folder_list[folder_index], buffer);
		if (!abspath) {
			// printf("[DEBUG] : could not find absolute path.\n");
			folder_index++;
			continue ;
		}
		directory = opendir(abspath);
		if (folder_index > 0 && command_structure->multiple_folders == 1)
			printf("\n");
		if (command_structure->multiple_folders == 1)
			printf("%s:\n", command_structure->folder_list[folder_index]);
		if (!directory) {
			// printf("[DEBUG] : ft_ls: cannot access '%s': No such file or directory\n", abspath);
			continue ;
		}
		while ((pDirent = readdir(directory))) {
			// printf("[DEBUG] : cycling dirent %s\n", pDirent->d_name);
			if (pDirent->d_name[0] == '.' && find_in_string(command_structure->flags, 'a') != 1)
				continue ;
			stat(pDirent->d_name, &fileStat);
			// if (S_ISDIR(fileStat.st_mode) && find_in_string(command_structure->flags, 'R') == 1)
			// 	printf("[DEBUG] : found a subdirectory to list recursively: %s\n", pDirent->d_name);
			print_file_entry(pDirent, command_structure);
		}
		if (find_in_string(command_structure->flags, 'l') != 1)
			printf("\n");
		folder_index++;
	}
	free_command_structure(command_structure);
	closedir(directory);
	return (0);
}